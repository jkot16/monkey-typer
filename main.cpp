#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Menu.h"
#include "ScoreBar.h"
#include "FileManager.h"
#include <filesystem>


auto getRandomWord(const std::vector<std::string>& wordList) -> std::string {
    if (wordList.empty()) {
        return "";
    } else {
        auto index = rand() % wordList.size();
        return wordList[index];
    }
}

auto isWordOfValidPosition(const sf::Text& newText, const std::vector<sf::Text>& existingWords) -> bool {
    return std::ranges::none_of(existingWords, [&newText](const sf::Text& word)-> bool {
        return newText.getGlobalBounds().intersects(word.getGlobalBounds());
    });
}

auto initializeTextTypeWords(std::vector<sf::Text>& textWords, const std::vector<std::string>& wordList, const sf::Font& font, sf::RenderWindow& window, sf::Clock& clock, float initialInterval, float regularInterval, int wordsNumber, int characterSize) -> void {
    static bool firstCall = true;
    float interval = firstCall ? initialInterval : regularInterval;
    if (clock.getElapsedTime().asSeconds() >= interval) {
        for (auto i = 0; i < wordsNumber; i++) {
            auto text = sf::Text(getRandomWord(wordList), font, characterSize);

            do {
                auto randomX = -text.getLocalBounds().width - (rand() % 400);
                auto randomY = rand() % (window.getSize().y - (int)(window.getSize().y * 0.1) - text.getCharacterSize());
                text.setPosition(randomX, randomY);
            } while (!isWordOfValidPosition(text, textWords));

            textWords.push_back(text);
        }
        clock.restart();
        firstCall = false;
    }
}

auto handleTextEnteredEvent(sf::RenderWindow& window, sf::Event& event, std::string& userInput) -> void {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == sf::Uint32(8) && !userInput.empty()) {
            userInput.pop_back();
        } else {
            auto enteredChar = char(event.text.unicode);
            if ((enteredChar >= 'A' && enteredChar <= 'Z') || (enteredChar >= 'a' && enteredChar <= 'z')) {
                userInput += enteredChar;
            }
        }
    }
}

auto resetGame(std::vector<sf::Text>& textWords, std::string& userInput, ScoreBar& scoreBar, int& wordsNumber) -> void {
    textWords.clear();
    userInput.clear();
    scoreBar.updateScore(0);
    scoreBar.resetClock();
    scoreBar.updateLives(10);
    wordsNumber = 2;
}
auto handleGameStateEvents(sf::RenderWindow& window, sf::Event& event, MenuState& menuState, MenuState& previousState, Menu& menu, std::vector<sf::Text>& textWords, std::string& userInput, ScoreBar& scoreBar, sf::Sound& click, float& speed, int& wordsNumber, std::string& playerName, bool& saveToLeaderboard, std::vector<LeaderboardEntry>& leaderboard, const sf::Font& font, int sizeToUse, FileManager& fileManager)-> void {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        switch (menuState) {
            case MenuState::SURVIVAL:
            case MenuState::SPEEDTEST:
            case MenuState::PRACTICE:
                previousState = menuState;
                menuState = MenuState::PAUSED;
                scoreBar.pauseClock();
                break;
            case MenuState::PAUSED:
                menuState = MenuState::MENU;
                break;
            case MenuState::NEWGAME:
            case MenuState::SELECTMODE:
            case MenuState::LEADERBOARD:
            case MenuState::SETTINGS:
                menuState = MenuState::MENU;
                break;
        }
    }

    if (menuState == MenuState::PAUSED && event.type == sf::Event::MouseButtonPressed) {
        menuState = previousState;
        scoreBar.resumeClock();
    }

    if (menuState == MenuState::MENU || menuState == MenuState::SELECTMODE || menuState == MenuState::SETTINGS) {
        auto newState = menu.handleMouseClick(event, window, menuState, click);
        if (newState == MenuState::NEWGAME) {
            resetGame(textWords, userInput, scoreBar, wordsNumber);
            menuState = MenuState::SELECTMODE;
        } else {
            menuState = newState;
        }
    } else if (menuState == MenuState::SURVIVAL || menuState == MenuState::SPEEDTEST || menuState == MenuState::PRACTICE) {
        handleTextEnteredEvent(window, event, userInput);
        if (menuState != MenuState::SURVIVAL && event.type == sf::Event::KeyPressed && event.key.control) {
            if (event.key.code == sf::Keyboard::Equal) { // ctrl+
                speed = std::min(1.2f, speed + 0.1f);
            } else if (event.key.code == sf::Keyboard::Hyphen) { // ctrl-
                speed = std::max(0.1f, speed - 0.1f);
            } else if (event.key.code == sf::Keyboard::S) {
                fileManager.saveWholeGameState("assets/savegame.txt", "assets/savegame_scores.txt", textWords, userInput, scoreBar, speed, menu.getSelectedFont(), menu.getSelectedSize(), menu.getSelectedTheme());
            } else if (event.key.code == sf::Keyboard::L) {
                auto loadedFont = FontType::ARIAL;
                auto loadedSize = SizeType::MEDIUM;
                auto loadedTheme = ThemeType::DEFAULT;
                fileManager.loadWholeGameState("assets/savegame.txt", "assets/savegame_scores.txt", textWords, userInput, scoreBar, font, sizeToUse, speed, loadedFont, loadedSize, loadedTheme);
                menu.setSelectedFont(loadedFont);
                menu.setSelectedSize(loadedSize);
                menu.setSelectedTheme(loadedTheme);
            }
        }
    } else if (menuState == MenuState::POSTSURVIVAL) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (menu.handleMouseClick(event, window, menuState, click) == MenuState::ENTERNAME) {
                menuState = MenuState::ENTERNAME;
            } else if (menu.handleMouseClick(event, window, menuState, click) == MenuState::MENU) {
                resetGame(textWords, userInput, scoreBar, wordsNumber);
                menuState = MenuState::MENU;
            }
        }
    } else if (menuState == MenuState::ENTERNAME) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == sf::Uint32(8) && !playerName.empty()) {
                playerName.pop_back();
            } else if (event.text.unicode == sf::Uint32(13) || event.text.unicode == sf::Uint32(10)) { // enter
                saveToLeaderboard = true;
                menuState = MenuState::LEADERBOARD;
            } else {
                playerName += event.text.unicode;
            }
        }
    }
}




auto gameOn(sf::RenderWindow& window, std::vector<sf::Text>& textWords, const std::vector<std::string>& words, const sf::Font& font, const int characterSize, sf::Clock& clock, float initialInterval, float& regularInterval, int& wordsNumber, float speed, std::string& userInput, sf::Sound& sound, ScoreBar& scoreBar, const sf::Sprite& background, MenuState& menuState, sf::Clock& survivalClock) -> void {
    if (menuState == MenuState::SURVIVAL) {
        initializeTextTypeWords(textWords, words, font, window, clock, initialInterval, regularInterval, wordsNumber, characterSize);

        if (survivalClock.getElapsedTime().asSeconds() >= 8) {
            wordsNumber += 1;
            survivalClock.restart();
        }
    } else {
        initializeTextTypeWords(textWords, words, font, window, clock, initialInterval, regularInterval, wordsNumber, characterSize);
    }

    auto toErase = std::ranges::remove_if(textWords, [&](sf::Text& word) {
        word.move(speed, 0);
        if (userInput == word.getString() && word.getPosition().x >= 0) {
            sound.stop();
            sound.play();
            userInput.clear();
            scoreBar.updateScore(scoreBar.getScore() + 1);
            return true;
        }
        if (word.getPosition().x > window.getSize().x && menuState == MenuState::SURVIVAL) {
            scoreBar.updateLives(scoreBar.getLives() - 1);
            return true;
        }
        return word.getPosition().x > window.getSize().x;
    });
    textWords.erase(toErase.begin(), toErase.end());

    if (menuState == MenuState::SURVIVAL && scoreBar.getLives() <= 0) {
        menuState = MenuState::POSTSURVIVAL;
    }

    scoreBar.updateTime();
    scoreBar.updateWordCount(textWords.size());
    scoreBar.updateUserInput(userInput);
    window.draw(background);
    scoreBar.draw(window, menuState);

    bool wordHighlighted = false;



    for (const auto& word : textWords) {
        auto fullWord = word.getString();
        auto correctPartLength = 0;
        for (auto i = 0; i < fullWord.getSize(); i++) {
            if (userInput[i] == fullWord[i]) {
                correctPartLength++;
            } else {
                break;
            }
        }

        auto correctPart = fullWord.substring(0, correctPartLength);
        auto remainingPart = fullWord.substring(correctPartLength);

        auto correctText = sf::Text(correctPart, font, characterSize);
        correctText.setFillColor(sf::Color::White);
        correctText.setPosition(word.getPosition());

        auto remainingText = sf::Text(remainingPart, font, characterSize);
        remainingText.setFillColor(sf::Color(80, 84, 84));  // rgb gray code
        remainingText.setPosition(word.getPosition().x + correctText.getLocalBounds().width, word.getPosition().y);

        bool startsWithUserInput = (fullWord.substring(0, userInput.size()) == userInput);

        if (startsWithUserInput && !wordHighlighted) {
            window.draw(correctText);
            window.draw(remainingText);
            wordHighlighted = true;
        } else {
            auto wordText = sf::Text(fullWord, font, characterSize);
            wordText.setFillColor(sf::Color(80, 84, 84));
            wordText.setPosition(word.getPosition());
            window.draw(wordText);
        }
    }
}



auto main() -> int {
    srand(time(NULL));
    auto window = sf::RenderWindow(sf::VideoMode(1700, 800), "Monkey Typer", sf::Style::Close);
    window.setFramerateLimit(200);
    std::filesystem::current_path("..");

    auto backgroundTexture = sf::Texture();
    if (!backgroundTexture.loadFromFile("assets/pokemon.png")) {
        return -1;
    }
    auto background = sf::Sprite();
    background.setTexture(backgroundTexture);

    auto buffer1 = sf::SoundBuffer();
    if (!buffer1.loadFromFile("assets/check.wav")) {
        return -1;
    }
    auto check = sf::Sound();
    check.setBuffer(buffer1);

    auto buffer2 = sf::SoundBuffer();
    if (!buffer2.loadFromFile("assets/mcClick.wav")) {
        return -1;
    }
    auto click = sf::Sound();
    click.setBuffer(buffer2);

    auto font = sf::Font();
    if (!font.loadFromFile("assets/arial.ttf")) {
        return -1;
    }

    auto menu = Menu(font, window);
    auto menuState = MenuState::MENU;

    auto userInput = std::string();
    auto scoreBar = ScoreBar(font, window, userInput);

    auto clock = sf::Clock();
    auto survivalClock = sf::Clock();
    auto initialInterval = float(2.0);
    auto regularInterval = float(4.0);
    auto wordSpeed = float(0.7);
    auto wordsNumber = 2;

    auto monkeyTexture = sf::Texture();
    if (!monkeyTexture.loadFromFile("assets/monkeyC++.png")) {
        return -1;
    }
    auto sprite = sf::Sprite();
    sprite.setTexture(monkeyTexture);
    sprite.setScale(0.7f, 0.7f);
    sprite.setPosition((window.getSize().x / 2) - 800, (window.getSize().y / 2) - 400);

    auto monkeyLeaderboard = sf::Texture();
    if (!monkeyLeaderboard.loadFromFile("assets/monkeyLeader.png")) {
        return -1;
    }
    auto spriteLeaderboardMonkey = sf::Sprite();
    spriteLeaderboardMonkey.setTexture(monkeyLeaderboard);
    spriteLeaderboardMonkey.setScale(0.6f, 0.6f);
    spriteLeaderboardMonkey.setPosition(window.getSize().x / 2 - 850, window.getSize().y / 2 - 350);

    auto fileManager = FileManager();
    auto defaultWords = fileManager.loadWordsFromFile("assets/defaultWords.txt");
    auto programmingWords = fileManager.loadWordsFromFile("assets/programmingWords.txt");
    auto businessWords = fileManager.loadWordsFromFile("assets/businessWords.txt");

    auto textWords = std::vector<sf::Text>();
    auto previousState = MenuState::MENU;
    auto playerName = std::string();
    auto saveToLeaderboard = false;
    auto leaderboard = fileManager.loadLeaderboardFromFile("assets/leaderboard.txt");

    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            handleGameStateEvents(window, event, menuState, previousState, menu, textWords, userInput, scoreBar, click, wordSpeed, wordsNumber, playerName, saveToLeaderboard, leaderboard, font, 30, fileManager);
        }

        if (menuState == MenuState::LEADERBOARD) {
            window.clear(sf::Color(51, 119, 255));
            window.draw(spriteLeaderboardMonkey);
        } else if (menuState == MenuState::PAUSED || menuState == MenuState::ENTERNAME || menuState == MenuState::POSTSURVIVAL) {
            window.draw(background);
        }

        if (menuState == MenuState::MENU || menuState == MenuState::SELECTMODE || menuState == MenuState::SETTINGS) {
            window.clear(sf::Color(51, 119, 255));
            window.draw(sprite);
        }
        if (menuState == MenuState::MENU || menuState == MenuState::PAUSED || menuState == MenuState::SELECTMODE || menuState == MenuState::SETTINGS) {
            menu.draw(window, menuState);
        } else if (menuState == MenuState::PRACTICE || menuState == MenuState::SURVIVAL) {
            auto fontToUse = sf::Font();
            auto sizeToUse = int();
            auto wordsToUse = std::vector<std::string>();
            auto practiceWordSpeed = wordSpeed;

            if (menuState == MenuState::SURVIVAL) {
                fontToUse.loadFromFile("assets/arial.ttf");
                sizeToUse = 30;
                wordsToUse = defaultWords;
                practiceWordSpeed = 0.7f;
            } else {
                if (menu.getSelectedFont() == FontType::ARIAL) {
                    fontToUse.loadFromFile("assets/arial.ttf");
                } else if (menu.getSelectedFont() == FontType::PLAYFAIR) {
                    fontToUse.loadFromFile("assets/playfair.ttf");
                } else if (menu.getSelectedFont() == FontType::POETSEN) {
                    fontToUse.loadFromFile("assets/poetsen.ttf");
                }
                if (menu.getSelectedSize() == SizeType::SMALL) {
                    sizeToUse = 20;
                } else if (menu.getSelectedSize() == SizeType::MEDIUM) {
                    sizeToUse = 30;
                } else if (menu.getSelectedSize() == SizeType::LARGE) {
                    sizeToUse = 35;
                }
                if (menu.getSelectedTheme() == ThemeType::PROGRAMMING) {
                    wordsToUse = programmingWords;
                } else if (menu.getSelectedTheme() == ThemeType::BUSINESS) {
                    wordsToUse = businessWords;
                } else if (menu.getSelectedTheme() == ThemeType::DEFAULT) {
                    wordsToUse = defaultWords;
                }
            }

            gameOn(window, textWords, wordsToUse, fontToUse, sizeToUse, clock, initialInterval, regularInterval,
                   wordsNumber, practiceWordSpeed, userInput, check, scoreBar, background, menuState, survivalClock);
        } else if (menuState == MenuState::LEADERBOARD) {
            menu.displayLeaderboard(window, leaderboard);
        } else if (menuState == MenuState::POSTSURVIVAL) {
            menu.draw(window, menuState);
        } else if (menuState == MenuState::ENTERNAME) {
            auto enterNameText = sf::Text("Enter your name: " + playerName, font, 30);
            enterNameText.setFillColor(sf::Color(80, 84, 84));
            enterNameText.setPosition(window.getSize().x / 2 - enterNameText.getLocalBounds().width / 2, 200);
            enterNameText.setStyle(sf::Text::Bold);
            window.draw(enterNameText);
        }

        if (saveToLeaderboard) {
            leaderboard.push_back({playerName, scoreBar.getScore(), scoreBar.getTime()});
            std::ranges::sort(leaderboard.begin(), leaderboard.end(),[](const LeaderboardEntry &a, const LeaderboardEntry &b) {
                return a.words > b.words || (a.words == b.words && a.time > b.time);
            });
            if (leaderboard.size() > 10) {
                leaderboard.pop_back();
            }
            fileManager.saveLeaderboardToFile("assets/leaderboard.txt", leaderboard);
            saveToLeaderboard = false;
        }

        window.display();
    }
}