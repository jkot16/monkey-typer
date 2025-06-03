
#ifndef PROJECT_SCOREBAR_H
#define PROJECT_SCOREBAR_H
#include <SFML/Graphics.hpp>

enum class MenuState;

class ScoreBar {
    int score;
    int lives;
    int wordCount;
    sf::RectangleShape scoreBar;
    sf::Text scoreText;
    sf::Clock gameClock;
    sf::Time totalElapsedTime;
    sf::Clock pausedClock;
    sf::Text timeText;
    sf::Text wordCountText;
    bool isPaused;
    sf::Text userInputText;
    sf::Text livesText;

public:
    ScoreBar(const sf::Font& font, const sf::RenderWindow& window, std::string userInput, int userLives = 10);
    auto pauseClock() -> void;
    auto resumeClock() -> void;
    auto updateTime() -> void;
    auto resetClock() -> void;
    auto draw(sf::RenderWindow& window, MenuState menuState) const -> void;
    auto updateScore(int newScore) -> void;
    auto updateWordCount(int newWordCount) -> void;
    auto updateUserInput(const std::string& userInput) -> void;
    auto updateLives(int newLives) -> void;
    auto getScore() const -> int;
    auto getLives() const -> int;
    auto getTime() const -> int;
    auto saveScoreBarValuesToFile(const std::string& filename) const -> void;
    auto updateDisplayedValues() -> void;
    auto loadScoreBarValuesFromFile(const std::string& filename) -> void;
};



#endif
