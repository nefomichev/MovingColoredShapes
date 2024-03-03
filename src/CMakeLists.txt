add_executable(
    ${PROJECT_NAME} 
    main.cpp 
    GameEngine.hpp 
    MovingColoredShapeWithText.hpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    sfml-system 
    sfml-window 
    sfml-graphics
)