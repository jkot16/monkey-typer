#include "Menu.h"
#include <fmt/core.h>




Menu::Menu(const sf::Font &font, const sf::RenderWindow &window)
    : font(font), fontChoices{"Playfair", "Arial", "Poetsen"}, sizeChoices{"Small", "Medium", "Large"}, themeChoices{"Default", "Programming", "Business"}, selectedFont(FontType::ARIAL), selectedSize(SizeType::MEDIUM), selectedTheme(ThemeType::DEFAULT) {
    title.setFont(font);
    title.setCharacterSize(50);
    title.setStyle(sf::Text::Bold);
    title.setString("Monkey Typer");
    title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 100);

    newGameOption.setFont(font);
    newGameOption.setCharacterSize(30);
    newGameOption.setString("New Game");
    newGameOption.setPosition(window.getSize().x / 2 - newGameOption.getLocalBounds().width / 2, 250);

    leaderboardOption.setFont(font);
    leaderboardOption.setCharacterSize(30);
    leaderboardOption.setString("Leaderboard");
    leaderboardOption.setPosition(window.getSize().x / 2 - leaderboardOption.getLocalBounds().width / 2, 300);

    settingsOption.setFont(font);
    settingsOption.setCharacterSize(30);
    settingsOption.setString("Settings");
    settingsOption.setPosition(window.getSize().x / 2 - settingsOption.getLocalBounds().width / 2, 350);

    exitOption.setFont(font);
    exitOption.setCharacterSize(30);
    exitOption.setString("Exit");
    exitOption.setPosition(window.getSize().x / 2 - exitOption.getLocalBounds().width / 2, 400);

    pauseText.setFont(font);
    pauseText.setCharacterSize(20);
    pauseText.setStyle(sf::Text::Bold);
    pauseText.setString("Game Paused. Press ESC again to return to main menu or click anywhere on screen to resume.");
    pauseText.setPosition(window.getSize().x / 2 - pauseText.getLocalBounds().width / 2, window.getSize().y / 2);

    survivalOption.setFont(font);
    survivalOption.setCharacterSize(30);
    survivalOption.setString("Survival");
    survivalOption.setPosition(window.getSize().x / 2 - survivalOption.getLocalBounds().width / 2, 250);

    speedTestOption.setFont(font);
    speedTestOption.setCharacterSize(30);
    speedTestOption.setString("Speed Test");
    speedTestOption.setPosition(window.getSize().x / 2 - speedTestOption.getLocalBounds().width / 2, 300);

    practiceOption.setFont(font);
    practiceOption.setCharacterSize(30);
    practiceOption.setString("Practice");
    practiceOption.setPosition(window.getSize().x / 2 - practiceOption.getLocalBounds().width / 2, 350);

    modeText.setFont(font);
    modeText.setCharacterSize(40);
    modeText.setStyle(sf::Text::Bold);
    modeText.setString("Choose mode: ");
    modeText.setPosition(window.getSize().x / 2 - modeText.getLocalBounds().width / 2, 150);

    settingsTitle.setFont(font);
    settingsTitle.setCharacterSize(40);
    settingsTitle.setStyle(sf::Text::Bold);
    settingsTitle.setString("Settings:");
    settingsTitle.setPosition(window.getSize().x / 2 - settingsTitle.getLocalBounds().width / 2, 150);

    fontOption.setFont(font);
    fontOption.setCharacterSize(30);
    fontOption.setString("Font: Arial");
    fontOption.setPosition(window.getSize().x / 2 - fontOption.getLocalBounds().width / 2, 400);

    sizeOption.setFont(font);
    sizeOption.setCharacterSize(30);
    sizeOption.setString("Size: Medium");
    sizeOption.setPosition(window.getSize().x / 2 - sizeOption.getLocalBounds().width / 2, 450);

    themeOption.setFont(font);
    themeOption.setCharacterSize(30);
    themeOption.setString("Theme: Default");
    themeOption.setPosition(window.getSize().x / 2 - themeOption.getLocalBounds().width / 2, 500);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, 150);
    gameOverText.setFillColor(sf::Color(80, 84, 84));

    questionText.setFont(font);
    questionText.setCharacterSize(30);
    questionText.setString("Do you wish to save your score to the leaderboard?");
    questionText.setPosition(window.getSize().x / 2 - questionText.getLocalBounds().width / 2, 250);
    questionText.setFillColor(sf::Color(80, 84, 84));

    yesText.setFont(font);
    yesText.setCharacterSize(30);
    yesText.setString("Yes");
    yesText.setPosition(window.getSize().x / 2 - 100, 350);
    yesText.setFillColor(sf::Color(80, 84, 84));

    noText.setFont(font);
    noText.setCharacterSize(30);
    noText.setString("No");
    noText.setPosition(window.getSize().x / 2 + 50, 350);
    noText.setFillColor(sf::Color(80, 84, 84));

    updateSettingsText(window);
}
auto Menu::draw(sf::RenderWindow& window, MenuState menuState) const -> void {
    if (menuState == MenuState::PAUSED) {
        window.draw(pauseText);
    } else if (menuState == MenuState::MENU) {
        window.draw(title);
        window.draw(newGameOption);
        window.draw(leaderboardOption);
        window.draw(settingsOption);
        window.draw(exitOption);
    } else if (menuState == MenuState::SELECTMODE) {
        window.draw(modeText);
        window.draw(survivalOption);
        window.draw(speedTestOption);
        window.draw(practiceOption);
    } else if (menuState == MenuState::SETTINGS) {
        window.draw(settingsTitle);
        window.draw(fontOption);
        window.draw(sizeOption);
        window.draw(themeOption);
    } else if (menuState == MenuState::POSTSURVIVAL) {
        window.draw(gameOverText);
        window.draw(questionText);
        window.draw(yesText);
        window.draw(noText);
    }
}

auto Menu::nextFontType(FontType type) -> FontType {return FontType((int(type) + 1) % 3);}

auto Menu::nextSizeType(SizeType type) -> SizeType {return SizeType((int(type) + 1) % 3);}

auto Menu::nextThemeType(ThemeType type) -> ThemeType {return ThemeType((int(type) + 1) % 3);}

auto Menu::handleMouseClick(const sf::Event& event, sf::RenderWindow& window, MenuState menuState, sf::Sound& click) -> MenuState {
    if (event.type == sf::Event::MouseButtonPressed) {
        auto mousePosition = sf::Vector2f(float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y));
        if (menuState == MenuState::MENU) {
            click.stop();
            click.play();
            if (newGameOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::NEWGAME;
            }
            if (leaderboardOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::LEADERBOARD;
            }
            if (settingsOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::SETTINGS;
            }
            if (exitOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                window.close();
            }
        } else if (menuState == MenuState::SELECTMODE) {
            if (survivalOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::SURVIVAL;
            }
            if (speedTestOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::SPEEDTEST;
            }
            if (practiceOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::PRACTICE;
            }
        } else if (menuState == MenuState::PAUSED) {
            click.stop();
            click.play();
            return MenuState::NEWGAME;
        } else if (menuState == MenuState::SETTINGS) {
            if (fontOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                selectedFont = nextFontType(selectedFont);
                fontOption.setString(fmt::format("Font: {}", fontChoices[int(selectedFont)]));
            }
            if (sizeOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                selectedSize = nextSizeType(selectedSize);
                sizeOption.setString(fmt::format("Size: {}", sizeChoices[int(selectedSize)]));
            }
            if (themeOption.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                selectedTheme = nextThemeType(selectedTheme);
                themeOption.setString(fmt::format("Theme: {}", themeChoices[int(selectedTheme)]));
            }
            updateSettingsText(window);
        } else if (menuState == MenuState::POSTSURVIVAL) {
            if (yesText.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::ENTERNAME;
            } else if (noText.getGlobalBounds().contains(mousePosition)) {
                click.stop();
                click.play();
                return MenuState::MENU;
            }
        }
    }
    return menuState;
}


auto Menu::updateSettingsText(const sf::RenderWindow& window) -> void {
    fontOption.setPosition(window.getSize().x / 2 - fontOption.getLocalBounds().width / 2, 250);
    sizeOption.setPosition(window.getSize().x / 2 - sizeOption.getLocalBounds().width / 2, 300);
    themeOption.setPosition(window.getSize().x / 2 - themeOption.getLocalBounds().width / 2, 350);
}

auto Menu::getSelectedFont() const -> FontType {return selectedFont;}
auto Menu::getSelectedSize() const -> SizeType {return selectedSize;}
auto Menu::getSelectedTheme() const -> ThemeType {return selectedTheme;}
auto Menu::setSelectedFont(FontType font) -> void {
    selectedFont = font;
    fontOption.setString(fmt::format("Font: {}", fontChoices[int(selectedFont)]));
}

auto Menu::setSelectedSize(SizeType size) -> void{
    selectedSize = size;
    sizeOption.setString(fmt::format("Size: {}", sizeChoices[int(selectedSize)]));
}

auto Menu::setSelectedTheme(ThemeType theme) -> void {
    selectedTheme = theme;
    themeOption.setString(fmt::format("Theme: {}", themeChoices[int(selectedTheme)]));
}


auto Menu::displayLeaderboard(sf::RenderWindow& window, const std::vector<LeaderboardEntry>& leaderboard) const -> void {
    auto title = sf::Text("Leaderboard", font, 40);
    title.setStyle(sf::Text::Bold);
    title.setPosition(window.getSize().x / 2 - title.getLocalBounds().width / 2, 150);
    window.draw(title);

    auto startY = 250;
    auto rank = int(1);
    for (const auto& entry : leaderboard) {
        auto entryText = fmt::format("{}. {} - Score: {} - Time: {}s", rank, entry.name, entry.words, entry.time);
        auto text = sf::Text(entryText, font, 30);
        text.setPosition(window.getSize().x / 2 - text.getLocalBounds().width / 2, startY);
        window.draw(text);
        startY += 50;
        rank++;
    }
}
