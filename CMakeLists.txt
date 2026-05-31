cmake_minimum_required(VERSION 3.21)

project(Proje_Mayintarlasi_2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCES
    Proje_Mayintarlasi_2.cpp
    MyLibary.cpp
    MyLibary.hpp
    resource.h
)

if(WIN32)
    list(APPEND PROJECT_SOURCES Proje_Mayintarlasi_2.rc)
endif()

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
elseif(MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

foreach(ASSET_DIR IN ITEMS Jpg Font Ses)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DIR}")
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DIR}"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${ASSET_DIR}"
            COMMENT "Copying ${ASSET_DIR} assets"
        )
    endif()
endforeach()

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()
