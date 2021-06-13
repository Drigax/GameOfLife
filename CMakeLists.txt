
add_executable (gameOfLife gameOfLife.cpp)
add_custom_command(TARGET gameOfLife POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${sampleInput.txt ${RUNTIME_OUTPUT_DIRECTORY}
)