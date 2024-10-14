# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME Audio)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "audio.cpp")

# copy wav-files to output folder
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD      # Adds a pre-build event to ${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_SOURCE_DIR}/mani.wav"          # <--this is in-file
        $<TARGET_FILE_DIR:${TARGET_NAME}>)              # <--this is out-file path
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD      # Adds a pre-build event to ${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_SOURCE_DIR}/piano.wav"         # <--this is in-file
        $<TARGET_FILE_DIR:${TARGET_NAME}>)              # <--this is out-file path
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD      # Adds a pre-build event to ${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_SOURCE_DIR}/Ring09.wav"        # <--this is in-file
        $<TARGET_FILE_DIR:${TARGET_NAME}>)              # <--this is out-file path
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD      # Adds a pre-build event to ${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       # which executes "cmake - E copy_if_different..."
        "${CMAKE_CURRENT_SOURCE_DIR}/Ring10.wav"        # <--this is in-file
        $<TARGET_FILE_DIR:${TARGET_NAME}>)              # <--this is out-file path
