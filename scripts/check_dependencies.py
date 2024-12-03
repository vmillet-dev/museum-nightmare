import re
import json
import argparse
import requests
from pathlib import Path
from urllib.parse import urlparse

def parse_github_url(url):
    """Extract owner and repo from GitHub URL."""
    path = urlparse(url.strip('"')).path
    parts = path.strip('/').split('/')
    if len(parts) >= 2:
        return parts[-2], parts[-1].replace('.git', '')
    return None, None

def parse_dependencies(file_path):
    """Parse the dependencies.cmake file to extract repositories and tags."""
    # First find all version variables
    variable_pattern = re.compile(r"set\((\w+_VERSION)\s+\"([^\"]+)\"\)")

    # Modified pattern to specifically catch the v prefix before ${VERSION} variables
    declare_pattern = re.compile(
        r"FetchContent_Declare\(\s*(\w+).*?"
        r"GIT_REPOSITORY\s+([^\s]+).*?"
        r"GIT_TAG\s+((?:v\${[\w_]+}|\${[\w_]+}|v?[^\s]+))",
        re.DOTALL
    )

    variables = {}
    dependencies = []

    with open(file_path, "r") as f:
        content = f.read()
        # Extract version variables first
        for match in variable_pattern.finditer(content):
            var_name, version = match.groups()
            variables[var_name] = version

        # Extract FetchContent_Declare information
        for match in declare_pattern.finditer(content):
            name, repo, tag = match.groups()
            repo = repo.strip('"')

            # Check if it's a variable reference
            if "${" in tag:
                var_name = re.search(r'\${([\w_]+)}', tag).group(1)
                if var_name in variables:
                    version = variables[var_name]
                    # Check if original declaration had 'v' prefix
                    has_v_prefix = tag.startswith('v')
                    tag = f"v{version}" if has_v_prefix else version
                    dependencies.append({
                        "name": name,
                        "repo": repo,
                        "tag": tag,
                        "variable": var_name
                    })
            else:
                # Direct tag value
                dependencies.append({
                    "name": name,
                    "repo": repo,
                    "tag": tag.strip('"'),
                    "variable": None
                })

    return dependencies, content

def version_to_tuple(version):
    """Convert version string to comparable tuple, handling 'v' prefix."""
    # Remove 'v' prefix if present
    version = version.lstrip('v')
    # Extract numbers from version string
    numbers = re.findall(r'\d+', version)
    return tuple(map(int, numbers))

def get_latest_version(repo_url):
    """Get the latest version from a GitHub repository using the API."""
    owner, repo = parse_github_url(repo_url)
    if not owner or not repo:
        print(f"  Error: Could not parse GitHub URL: {repo_url}")
        return None

    try:
        # Use GitHub API to get tags
        api_url = f"https://api.github.com/repos/{owner}/{repo}/tags"
        headers = {"Accept": "application/vnd.github.v3+json"}

        response = requests.get(api_url, headers=headers)
        response.raise_for_status()

        # Get all tags and filter for valid semantic versions
        tags = response.json()
        valid_tags = []
        for tag in tags:
            tag_name = tag["name"]
            if re.match(r'^v?\d+\.\d+\.\d+$', tag_name):
                # Verify the tag actually exists by checking its commit
                commit_url = f"https://api.github.com/repos/{owner}/{repo}/git/refs/tags/{tag_name}"
                try:
                    commit_response = requests.get(commit_url, headers=headers)
                    commit_response.raise_for_status()
                    valid_tags.append(tag_name)
                except requests.RequestException:
                    print(f"  Warning: Tag {tag_name} exists but may be invalid")
                    continue

        if valid_tags:
            return sorted(valid_tags, key=version_to_tuple)[-1]
        print(f"  Warning: No valid semantic version tags found for {owner}/{repo}")
        return None
    except requests.RequestException as e:
        print(f"  Error fetching tags: {str(e)}")
        return None
    except Exception as e:
        print(f"  Unexpected error: {str(e)}")
        return None

def update_file(content, dependencies, file_path):
    """Update the dependencies.cmake file with the new versions."""
    updated = False
    for dep in dependencies:
        if dep.get("latest_version") and version_to_tuple(dep["latest_version"]) > version_to_tuple(dep["tag"]):
            print(f"Updating {dep['name']} to version {dep['latest_version']}...")
            # Update either the GIT_TAG or the corresponding variable
            if dep["variable"]:
                # Remove 'v' prefix for variable value
                new_version = dep["latest_version"].lstrip('v')
                # Update the version variable
                content = re.sub(
                    rf'set\({dep["variable"]}\s+"[^"]+"\)',
                    f'set({dep["variable"]} "{new_version}")',
                    content
                )
                # Update the GIT_TAG if it uses the variable
                if "v${" in content:
                    content = re.sub(
                        rf'GIT_TAG\s+v\${{{dep["variable"]}}}',
                        f'GIT_TAG v${{{dep["variable"]}}}',
                        content
                    )
                else:
                    content = re.sub(
                        rf'GIT_TAG\s+\${{{dep["variable"]}}}',
                        f'GIT_TAG ${{{dep["variable"]}}}',
                        content
                    )
            else:
                # For direct tags, maintain the same prefix style as before
                had_v_prefix = dep["tag"].startswith('v')
                new_tag = dep["latest_version"] if had_v_prefix else dep["latest_version"].lstrip('v')
                content = re.sub(
                    rf"GIT_TAG\s+{re.escape(dep['tag'])}",
                    f"GIT_TAG {new_tag}",
                    content
                )
            updated = True

    if updated:
        with open(file_path, "w") as f:
            f.write(content)
        print("File updated successfully.")
    else:
        print("No updates were made.")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dependencies-file", required=True, help="Path to dependencies.cmake file")
    args = parser.parse_args()

    file_path = Path(args.dependencies_file)
    if not file_path.exists():
        print(f"File not found: {file_path}")
        return

    dependencies, content = parse_dependencies(file_path)
    if not dependencies:
        print("No dependencies found in the specified file.")
        return

    print("Checking for updates...")
    for dep in dependencies:
        print(f"Checking {dep['name']} ({dep['repo']})...")
        latest_version = get_latest_version(dep["repo"])
        dep["latest_version"] = latest_version

        if latest_version and version_to_tuple(latest_version) > version_to_tuple(dep["tag"]):
            print(f"  Update available for {dep['name']}: {latest_version} (current: {dep['tag']})")
        elif latest_version:
            print(f"  {dep['name']} is up-to-date (current version: {dep['tag']})")
        else:
            print(f"  Could not determine the latest version for {dep['name']} (current version: {dep['tag']})")

    # Update the file with new versions
    update_file(content, dependencies, file_path)

if __name__ == "__main__":
    main()
