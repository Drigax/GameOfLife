
add_executable (gameOfLife gameOfLife.cpp)
add_custom_command(TARGET gameOfLife POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sampleInput.txt sampleInput.txt
)