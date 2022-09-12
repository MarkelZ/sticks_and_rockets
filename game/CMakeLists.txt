add_executable(game
    main.cpp
    bomb.cpp
    entity.cpp
    game.cpp
    particleeffect.cpp
    player.cpp
    ragdoll.cpp
    physics/dynamicobject.cpp
    physics/rigidlink.cpp
    physics/shape.cpp
    physics/simulation.cpp
    physics/trigger.cpp
    physics/triggerarea.cpp
    physics/vectormath.cpp
    physics/vertex.cpp
    utils/debugwriter.cpp
    utils/input.cpp)
target_link_libraries(game sfml-graphics sfml-audio)

# Copy required files to build folder after building
# https://thomas.trocha.com/blog/cmake--copy-files-after-build/
add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/content
    ${CMAKE_CURRENT_BINARY_DIR}/content
)

add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_BINARY_DIR}/models
)