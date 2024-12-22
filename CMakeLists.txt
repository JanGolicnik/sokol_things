cmake_minimum_required(VERSION 3.10)

project(sokol_test)

set(CMAKE_C_STANDARD 11)

# generate shader headers
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
file(GLOB SHADER_FILES ${SHADER_DIR}/*.glsl)
set(SOKOL_SHDC_PATH ${CMAKE_SOURCE_DIR}/tools/sokol-shdc.exe)
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    set(HEADER_FILE ${SHADER_DIR}/${SHADER_NAME}.h)

    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${SOKOL_SHDC_PATH} --input ${SHADER} --output ${HEADER_FILE} --slang glsl430
        DEPENDS ${SHADER}
        VERBATIM
    )

    list(APPEND GENERATED_HEADERS ${HEADER_FILE})
endforeach()

add_custom_target(GenerateShaders ALL DEPENDS ${GENERATED_HEADERS})

set(SOURCES main.c)
add_executable(sokol_test ${SOURCES})

target_include_directories(sokol_test PRIVATE ${SHADER_DIR})
