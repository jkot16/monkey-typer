#include "FileManager.h"
#include <fstream>
#include <SFML/Graphics.hpp>


auto FileManager::loadWordsFromFile(const std::string& filename) -> std::vector<std::string> {
    auto file = std::fstream(filename);
    auto word = std::string();
    auto words = std::vector<std::string>();
    while (file >> word) {
        words.push_back(word);
    }
    return words;
}

auto FileManager::loadLeaderboardFromFile(const std::string& filename) -> std::vector<LeaderboardEntry> {
    auto file = std::fstream(filename);
    auto leaderboard = std::vector<LeaderboardEntry>();
    auto name = std::string();
    auto words = int();
    auto time = int();
    while (file >> name >> words >> time) {
        leaderboard.push_back({name, words, time});
    }
    return leaderboard;
}

auto FileManager::saveLeaderboardToFile(const std::string& filename, const std::vector<LeaderboardEntry>& leaderboard) -> void {
    auto file = std::fstream(filename);
    for (const auto& entry : leaderboard) {
        file << entry.name << " " << entry.words << " " << entry.time << "\n";
    }
}
auto FileManager::saveWholeGameState(const std::string& filename, const std::string& scoresFilename, const std::vector<sf::Text>& textWords, const std::string& userInput, const ScoreBar& scoreBar, float speed, FontType font, SizeType size, ThemeType theme) -> void {
    auto file = std::fstream(filename);
    file << userInput << '\n';
    file << textWords.size() << '\n';
    for (const auto& word : textWords) {
        file << word.getString().toAnsiString() << '\n' << word.getPosition().x << ' ' << word.getPosition().y << '\n';
    }
    file << speed << '\n';
    file << int(font) << '\n';
    file << int(size) << '\n';
    file << int(theme) << '\n';
    scoreBar.saveScoreBarValuesToFile(scoresFilename);
}


auto FileManager::loadWholeGameState(const std::string& filename, const std::string& scoresFilename, std::vector<sf::Text>& textWords, std::string& userInput, ScoreBar& scoreBar, const sf::Font& font, int& characterSize, float& speed, FontType& loadedFont, SizeType& loadedSize, ThemeType& loadedTheme) -> void {
    auto file = std::fstream(filename);
    std::getline(file, userInput);
    auto textWordCount = int();
    file >> textWordCount;
    textWords.clear();
    for (auto i = 0; i < textWordCount; i++) {
        auto word = std::string();
        auto posX = float();
        auto posY = float();
        file >> word >> posX >> posY;

        auto text = sf::Text(word, font, characterSize);
        text.setPosition(posX, posY);

        textWords.push_back(text);
    }
    file >> speed;
    auto fontType = int();
    auto sizeType = int();
    auto themeType = int();
    file >> fontType >> sizeType >> themeType;
    loadedFont = FontType(fontType);
    loadedSize = SizeType(sizeType);
    loadedTheme = ThemeType(themeType);

    scoreBar.loadScoreBarValuesFromFile(scoresFilename);
}