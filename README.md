# Museum Nightmare

A 2D game built with SFML where you navigate through a haunted museum.

## Prerequisites

- CMake (>= 3.16)
- C++23 compatible compiler
- X11 development libraries
- OpenGL development libraries
- OpenAL development libraries
- Vorbis and FLAC audio codecs
- FreeType library

On Ubuntu/Debian, you can install the required dependencies with:
```bash
sudo apt-get update && sudo apt-get install -y \
    build-essential \
    cmake \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libgl1-mesa-dev \
    libudev-dev \
    libopenal-dev \
    libvorbis-dev \
    libflac-dev \
    libfreetype-dev
```

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/vmillet-dev/museum-nightmare.git
cd museum-nightmare
```

2. Create and navigate to the build directory:
```bash
mkdir build && cd build
```

3. Configure and build the project:
```bash
cmake ..
make
```

## Running the Game

After building, you can run the game from the build directory:
```bash
./sfml_game
```

## Controls

- Z: Move Up
- S: Move Down
- Q: Move Left
- D: Move Right
- ESC: Exit Game

## Project Structure

The project uses CMake for build configuration and depends on the following libraries (automatically fetched during build):

- SFML 2.6.2: Graphics, window management, and input handling
- toml++ 3.4.0: Configuration file parsing
- spdlog 1.12.0: Logging system

## Notes

- The game requires a working X11 display server to run
- Controller support is available and can be configured in the game settings
