cmake_minimum_required(VERSION 3.22)
project(nexus_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MOD_ID "nexus_client" CACHE STRING "")
set(MOD_NAME "NexusClient" CACHE STRING "")
set(MOD_AUTHOR "NexusTeam" CACHE STRING "")
set(MOD_VERSION "1.0.0" CACHE STRING "")
set(MOD_LIBRARY_NAME "nexus_client" CACHE STRING "")
set(MOD_MINECRAFT_VERSIONS "[\"1.21.*\",\"1.26.*\"]" CACHE STRING "")
set(MOD_ICON "" CACHE STRING "")

include(FetchContent)

FetchContent_Declare(preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
    SOURCE_SUBDIR cmake/preloader-sdk
)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_Declare(boost_pfr
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG boost-1.88.0
)
FetchContent_Declare(magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7
)
FetchContent_MakeAvailable(preloader_android nlohmann_json boost_pfr magic_enum)
FetchContent_GetProperties(preloader_android)
set(LEVI_PRELOADER_INCLUDE_DIR "${preloader_android_SOURCE_DIR}/include")

# Skip config generator on non-Android
if(NOT ANDROID)
    return()
endif()

if(NOT DEFINED LEVI_PACKAGE_CONFIG_DIR)
    set(LEVI_PACKAGE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config" CACHE PATH "")
endif()

option(LEVI_LINK_PRELOADER "" ON)
if(LEVI_LINK_PRELOADER AND NOT TARGET preloader)
    add_subdirectory("${preloader_android_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_preloader_android")
endif()

add_library(${MOD_LIBRARY_NAME} SHARED
    src/main.cpp
    src/mod/NexusMod.cpp
    src/mod/modules/Combat.cpp
    src/mod/modules/Movement.cpp
    src/mod/modules/Render.cpp
    src/mod/modules/Player.cpp
    src/mod/modules/World.cpp
    src/mod/modules/Misc.cpp
)

set_target_properties(${MOD_LIBRARY_NAME} PROPERTIES OUTPUT_NAME "${MOD_LIBRARY_NAME}")
target_include_directories(${MOD_LIBRARY_NAME} PRIVATE src "${LEVI_PRELOADER_INCLUDE_DIR}")
target_compile_options(${MOD_LIBRARY_NAME} PRIVATE $<$<CONFIG:Release>:-fvisibility=hidden -fvisibility-inlines-hidden>)
target_link_options(${MOD_LIBRARY_NAME} PRIVATE "-Wl,-z,max-page-size=16384")

find_library(log-lib log REQUIRED)
target_link_libraries(${MOD_LIBRARY_NAME} PRIVATE
    ${log-lib}
    nlohmann_json::nlohmann_json
    Boost::pfr
    magic_enum::magic_enum
)
if(TARGET preloader)
    target_link_libraries(${MOD_LIBRARY_NAME} PRIVATE preloader)
else()
    target_link_options(${MOD_LIBRARY_NAME} PRIVATE "-Wl,--unresolved-symbols=ignore-all")
endif()

if(CMAKE_STRIP)
    add_custom_command(TARGET ${MOD_LIBRARY_NAME} POST_BUILD
        COMMAND "${CMAKE_STRIP}" --strip-unneeded "$<TARGET_FILE:${MOD_LIBRARY_NAME}>"
    )
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/manifest.json.in" "${CMAKE_CURRENT_BINARY_DIR}/manifest.json" @ONLY)

set(LEVI_PACKAGE_ROOT "${CMAKE_CURRENT_BINARY_DIR}/package")
set(LEVI_PACKAGE_MOD_DIR "${LEVI_PACKAGE_ROOT}/${MOD_ID}")
set(LEVI_PACKAGE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}-${MOD_VERSION}-${CMAKE_ANDROID_ARCH_ABI}.levipack")

add_custom_target(levi_package
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${LEVI_PACKAGE_ROOT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${LEVI_PACKAGE_MOD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:${MOD_LIBRARY_NAME}>" "${LEVI_PACKAGE_MOD_DIR}/lib${MOD_LIBRARY_NAME}.so"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_BINARY_DIR}/manifest.json" "${LEVI_PACKAGE_MOD_DIR}/manifest.json"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${LEVI_PACKAGE_CONFIG_DIR}" "${LEVI_PACKAGE_MOD_DIR}/config"
    COMMAND "${CMAKE_COMMAND}" -E chdir "${LEVI_PACKAGE_ROOT}" "${CMAKE_COMMAND}" -E tar cf "${LEVI_PACKAGE_FILE}" --format=zip "${MOD_ID}"
    DEPENDS ${MOD_LIBRARY_NAME}
    COMMENT "Packaging NexusClient for LeviLauncher"
)
