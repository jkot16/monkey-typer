#include "ScoreBar.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fmt/core.h>
#include <fstream>
#include "Menu.h"


ScoreBar::ScoreBar(const sf::Font& font, const sf::RenderWindow& window, std::string userInput, int userLives)
        : score(0), lives(userLives), wordCount(0), scoreBar(sf::Vector2f(window.getSize().x, window.getSize().y * 0.1)), totalElapsedTime(sf::Time::Zero), isPaused(false) {

    scoreBar.setPosition(0, window.getSize().y - scoreBar.getSize().y);
    scoreBar.setFillColor(sf::Color(137, 207, 240));

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(window.getSize().x / 2 + 250, window.getSize().y - scoreBar.getSize().y + 10);
    scoreText.setString(fmt::format("Score: {}", score));

    timeText.setFont(font);
    timeText.setCharacterSize(30);
    timeText.setStyle(sf::Text::Bold);
    timeText.setPosition(window.getSize().x - 400, window.getSize().y - scoreBar.getSize().y + 10);

    wordCountText.setFont(font);
    wordCountText.setCharacterSize(30);
    wordCountText.setStyle(sf::Text::Bold);
    wordCountText.setPosition(window.getSize().x - 200, window.getSize().y - scoreBar.getSize().y + 10);

    userInputText.setFont(font);
    userInputText.setCharacterSize(30);
    userInputText.setStyle(sf::Text::Bold);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setPosition(10, window.getSize().y - scoreBar.getSize().y + 10);
    userInputText.setString(fmt::format("[{}]", userInput));

    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setStyle(sf::Text::Bold);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(window.getSize().x / 2 + 60, window.getSize().y - scoreBar.getSize().y + 10);
    livesText.setString(fmt::format("Lives: {}", lives));
}
auto ScoreBar::pauseClock() -> void {
    if (!isPaused) {
        totalElapsedTime += gameClock.getElapsedTime();
        isPaused = true;
    }
}

auto ScoreBar::resumeClock() -> void {
    if (isPaused) {
        gameClock.restart();
        isPaused = false;
    }
}

auto ScoreBar::updateTime() -> void {
    auto currentTime = sf::Time();
    if (isPaused) {
        currentTime = totalElapsedTime;
    } else {
        currentTime = totalElapsedTime + gameClock.getElapsedTime();
    }
    auto seconds = int(currentTime.asSeconds());
    timeText.setString(fmt::format("Time: {}s", seconds));
}

auto ScoreBar::resetClock() -> void {
    gameClock.restart();
    totalElapsedTime = sf::Time::Zero;
    isPaused = false;
}

auto ScoreBar::draw(sf::RenderWindow& window, MenuState menuState) const -> void {
    window.draw(scoreBar);
    window.draw(scoreText);
    window.draw(timeText);
    window.draw(wordCountText);
    window.draw(userInputText);
    if (menuState == MenuState::SURVIVAL) {
        window.draw(livesText);
    }
}

auto ScoreBar::updateScore(int newScore) -> void {
    score = newScore;
    scoreText.setString(fmt::format("Score: {}", score));
}

auto ScoreBar::updateWordCount(int newWordCount) -> void {
    wordCount = newWordCount;
    wordCountText.setString(fmt::format("Words: {}", wordCount));
}

auto ScoreBar::updateUserInput(const std::string& userInput) -> void {
    userInputText.setString(fmt::format("[{}]", userInput));
}

auto ScoreBar::updateLives(int newLives) -> void {
    lives = newLives;
    livesText.setString(fmt::format("Lives: {}", lives));
}

auto ScoreBar::getScore() const -> int {
    return score;
}

auto ScoreBar::getLives() const -> int {
    return lives;
}

auto ScoreBar::getTime() const -> int {
    return int(totalElapsedTime.asSeconds() + gameClock.getElapsedTime().asSeconds());
}


auto ScoreBar::saveScoreBarValuesToFile(const std::string& filename) const -> void {
    auto file = std::fstream(filename);
    auto currentTime = isPaused ? totalElapsedTime : totalElapsedTime + gameClock.getElapsedTime();
    file << score << '\n' << currentTime.asSeconds() << '\n' << wordCount << '\n';


}

auto ScoreBar::updateDisplayedValues() -> void {
    scoreText.setString(fmt::format("Score: {}", score));
    timeText.setString(fmt::format("Time: {}s", int(totalElapsedTime.asSeconds() + gameClock.getElapsedTime().asSeconds())));
    wordCountText.setString(fmt::format("Words: {}", wordCount));
}

auto ScoreBar::loadScoreBarValuesFromFile(const std::string& filename) -> void {
    auto file = std::fstream(filename);
    auto elapsedTime = float();
    file >> score >> elapsedTime >> wordCount;
    totalElapsedTime = sf::seconds(elapsedTime);
    gameClock.restart();
    isPaused = false;
    updateDisplayedValues();

}