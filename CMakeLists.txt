cmake_minimum_required(VERSION 3.28)
project(floc VERSION 0.0.3)

include(FetchContent)

option(ENABLE_ADDRSAN "Enable the address sanitizer" OFF)
option(ENABLE_UBSAN "Enable the ub sanitizer" OFF)
option(ENABLE_TSAN "Enable the thread data race sanitizer" OFF)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/)
file(GLOB_RECURSE PROJECT_BUILD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(floc ${PROJECT_BUILD_FILES})

add_subdirectory(lib/blt)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(floc ${BLT_TARGET})
target_link_libraries(floc
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)

target_compile_options(floc PRIVATE -Wall -Werror -Wpedantic -Wno-comment)
target_link_options(floc PRIVATE -Wall -Werror -Wpedantic -Wno-comment)

if (${ENABLE_ADDRSAN} MATCHES ON)
    target_compile_options(floc PRIVATE -fsanitize=address)
    target_link_options(floc PRIVATE -fsanitize=address)
endif ()

if (${ENABLE_UBSAN} MATCHES ON)
    target_compile_options(floc PRIVATE -fsanitize=undefined)
    target_link_options(floc PRIVATE -fsanitize=undefined)
endif ()

if (${ENABLE_TSAN} MATCHES ON)
    target_compile_options(floc PRIVATE -fsanitize=thread)
    target_link_options(floc PRIVATE -fsanitize=thread)
endif ()
