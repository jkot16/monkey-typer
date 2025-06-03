

#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <fmt/core.h>

enum class MenuState {
    MENU,
    NEWGAME,
    LEADERBOARD,
    SETTINGS,
    PAUSED,
    SURVIVAL,
    SPEEDTEST,
    PRACTICE,
    SELECTMODE,
    ENTERNAME,
    POSTSURVIVAL
};

enum class FontType {
    PLAYFAIR,
    ARIAL,
    POETSEN
};

enum class SizeType {
    SMALL,
    MEDIUM,
    LARGE
};

enum class ThemeType {
    DEFAULT,
    PROGRAMMING,
    BUSINESS
};

struct LeaderboardEntry {
    std::string name;
    int words;
    int time;
};

class Menu {
    sf::Font font;
    sf::Text title;
    sf::Text newGameOption;
    sf::Text leaderboardOption;
    sf::Text settingsOption;
    sf::Text exitOption;
    sf::Text pauseText;
    sf::Text survivalOption;
    sf::Text speedTestOption;
    sf::Text practiceOption;
    sf::Text modeText;
    sf::Text settingsTitle;
    sf::Text fontOption;
    sf::Text sizeOption;
    sf::Text themeOption;
    sf::Text gameOverText;
    sf::Text questionText;
    sf::Text yesText;
    sf::Text noText;
    std::vector<std::string> fontChoices;
    std::vector<std::string> sizeChoices;
    std::vector<std::string> themeChoices;
    FontType selectedFont;
    SizeType selectedSize;
    ThemeType selectedTheme;

public:
    Menu(const sf::Font& font, const sf::RenderWindow& window);
    auto draw(sf::RenderWindow& window, MenuState menuState) const -> void;
    auto nextFontType(FontType type) -> FontType;
    auto nextSizeType(SizeType type) -> SizeType;
    auto nextThemeType(ThemeType type) -> ThemeType;
    auto handleMouseClick(const sf::Event& event, sf::RenderWindow& window, MenuState menuState, sf::Sound& click) -> MenuState;
    auto updateSettingsText(const sf::RenderWindow& window) -> void;
    auto getSelectedFont() const -> FontType;
    auto getSelectedSize() const -> SizeType;
    auto getSelectedTheme() const -> ThemeType;
    auto setSelectedFont(FontType font) -> void;
    auto setSelectedSize(SizeType size) -> void;
    auto setSelectedTheme(ThemeType theme) -> void;
    auto displayLeaderboard(sf::RenderWindow& window, const std::vector<LeaderboardEntry>& leaderboard) const -> void;
};

#endif
