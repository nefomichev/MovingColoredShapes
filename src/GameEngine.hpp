#include "MovingColoredShapeWithText.hpp"
#include "fstream"
#include "iostream"
#include "map"
#include "functional"
#include <SFML/System/Clock.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#ifndef MOVINGSHAPES_GAMEENGINE_HPP
#define MOVINGSHAPES_GAMEENGINE_HPP

using uint = unsigned int;

class Engine

{
public:
  std::shared_ptr<sf::RenderWindow> gameWindow;
  sf::Clock deltaClock;

public:
  explicit Engine(const std::string &configFileName) {
    loadFromFile(configFileName);
  }

  void loadFromFile(const std::string &filename) {
    std::string option_name;
    std::ifstream fin = readConfigFromFile(filename);

    while (fin >> option_name) {
      if (option_name == "Window") {
        parseWindowSettings(fin);
      }

      if (option_name == "Font") {
        parseFont(fin);
        parseFontSettings(fin);
      }

      if (m_shapeCreationFunctions.count(option_name) > 0) {
        parseAndLoadShape(fin, option_name);
      }
    }
  };

  void renderFrame() {
    gameWindow->clear();
    for (auto &shape : m_movingColorShapes) {
      gameWindow->draw(*shape.getShape());
      gameWindow->draw(*shape.getShapeText());
    }
    ImGui::SFML::Render(*gameWindow);
    gameWindow->display();
  }

  void updateFrame() {
    for (auto &shape : m_movingColorShapes) {
      shape.windowBounce({m_windowWidth, m_windowHeight});
      shape.move();
    }
    ImGui::SFML::Update(*gameWindow, deltaClock.restart());
    ImGui::ShowDemoWindow();
  }

  void createGameWindow() {
    gameWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(m_windowWidth, m_windowHeight), "MovingColoredShapes");
    gameWindow->setFramerateLimit(60);
    ImGui::SFML::Init(*gameWindow);
  }

  void lookForEvents() const {
    sf::Event event{};
    while (gameWindow->pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed) {
        gameWindow->close();
      }
    }
  }

private:
  using ShapeLoaderFunction =
      std::function<std::shared_ptr<sf::Shape>(std::ifstream &)>;

  uint m_windowWidth{};
  uint m_windowHeight{};
  sf::Font m_textFont{};
  sf::Text m_WindowDefaultText{};
  std::vector<MovingColoredShapeWithText> m_movingColorShapes{};
  std::map<std::string, ShapeLoaderFunction> m_shapeCreationFunctions = {
      {"Circle", loadCircle}, {"Rectangle", loadRectangle}};

private:
  static std::shared_ptr<sf::Shape> loadCircle(std::ifstream &fin) {
    float radius;
    fin >> radius;
    return std::make_shared<sf::CircleShape>(radius);
  }

  static std::shared_ptr<sf::Shape> loadRectangle(std::ifstream &fin) {
    float width;
    float height;
    fin >> height >> width;
    return std::make_shared<sf::RectangleShape>(sf::Vector2f(height, width));
  }

  void parseWindowSettings(std::ifstream &fin) {
    fin >> m_windowWidth >> m_windowHeight;
  }

  void parseFont(std::ifstream &fin) {
    std::string fontPath;
    fin >> fontPath;

    loadFont(fontPath);
  }

  void loadFont(const std::string &fontPath) {
    if (!m_textFont.loadFromFile(fontPath)) {
      throw std::runtime_error("No Font was found");
    } else {
      m_WindowDefaultText.setFont(m_textFont);
    }
  }

  void parseFontSettings(std::ifstream &fin) {
    uint parsedFontSize, rColor, gColor, bColor;
    fin >> parsedFontSize >> rColor >> gColor >> bColor;
    auto fontColor = sf::Color(rColor, gColor, bColor);

    loadFontSettings(fontColor, parsedFontSize);
  }

  void loadFontSettings(const sf::Color &fontColor, uint fontSize) {
    try {
      m_WindowDefaultText.setCharacterSize(fontSize);
      m_WindowDefaultText.setFillColor(fontColor);
    } catch (const std::exception &e) {
      std::cerr << "Error setting font settings: " << e.what() << std::endl;
    }
  }

  void parseAndLoadShape(std::ifstream &fin, const std::string &shapeType) {
    std::string shapeName;
    float initX, initY, initSX, initSY;
    uint rColor, gColor, bColor;

    fin >> shapeName >> initX >> initY >> initSX >> initSY >> rColor >>
        gColor >> bColor;
    auto shape = m_shapeCreationFunctions[shapeType](fin);
    auto shapeText = createShapeTextFromDefaultTemplate(shapeName);

    m_movingColorShapes.emplace_back(
        shape, shapeText, sf::Color(rColor, gColor, bColor),
        sf::Vector2f{initX, initY}, sf::Vector2f{initSX, initSY});
  }

  std::shared_ptr<sf::Text>
  createShapeTextFromDefaultTemplate(const std::string &shapeName) {
    auto shapeText = m_WindowDefaultText;
    shapeText.setString(shapeName);

    return std::make_shared<sf::Text>(shapeText);
  }

  static std::ifstream readConfigFromFile(const std::string &filename) {
    std::ifstream fin(filename);

    if (fin.fail()) {
      throw std::runtime_error("Config was not found");
    } else {
      return fin;
    }
  }
};
#endif // MOVINGSHAPES_GAMEENGINE_HPP
