#ifndef PROJECT_FILEMANAGER_H
#define PROJECT_FILEMANAGER_H

#include <string>
#include <vector>
#include "ScoreBar.h"
#include "Menu.h"

class FileManager {
public:
    auto loadWordsFromFile(const std::string& filename) -> std::vector<std::string>;
    auto loadLeaderboardFromFile(const std::string& filename) -> std::vector<LeaderboardEntry>;
    auto saveLeaderboardToFile(const std::string& filename, const std::vector<LeaderboardEntry>& leaderboard) -> void;
    auto saveWholeGameState(const std::string& filename, const std::string& scoresFilename, const std::vector<sf::Text>& textWords, const std::string& userInput, const ScoreBar& scoreBar, float speed, FontType font, SizeType size, ThemeType theme) -> void;
    auto loadWholeGameState(const std::string& filename, const std::string& scoresFilename, std::vector<sf::Text>& textWords, std::string& userInput, ScoreBar& scoreBar, const sf::Font& font, int& characterSize, float& speed, FontType& loadedFont, SizeType& loadedSize, ThemeType& loadedTheme) -> void;
};

#endif
