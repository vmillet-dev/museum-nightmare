include(${CMAKE_CURRENT_LIST_DIR}/configure_ext_libs.cmake)
include(FetchContent)

configure_curl_build_options()

set(NLOHMANN_JSON_VERSION "3.11.3")
set(CURL_VERSION "curl-8_11_1")

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v${NLOHMANN_JSON_VERSION}
)

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG ${CURL_VERSION}
)

FetchContent_MakeAvailable(nlohmann_json curl)