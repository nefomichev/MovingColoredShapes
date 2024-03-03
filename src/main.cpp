#include "GameEngine.hpp"
#include "MovingColoredShapeWithText.hpp"

int main() {
  auto engine = Engine("../../../config.txt");
  engine.createGameWindow();

  while (engine.gameWindow->isOpen()) {
    engine.lookForEvents();
    engine.updateFrame();
    engine.renderFrame();
  }
  return 0;
}
