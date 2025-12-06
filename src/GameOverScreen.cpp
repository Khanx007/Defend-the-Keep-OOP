// GameOverScreen.cpp
#include "GameOverScreen.hpp"

void GameOverScreen::init(sf::Font& font) {
    // Semi-transparent overlay
    overlay.setSize(sf::Vector2f(1280.f, 720.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));

    // "GAME OVER" title
    titleText.setFont(font);
    titleText.setString("GAME OVER");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Red);
    titleText.setPosition(420.f, 80.f);

    // Stats text (content set in updateStats)
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(420.f, 160.f);

    // Restart button rectangle
    buttonRestartRect.setSize({220.f, 50.f});
    buttonRestartRect.setFillColor(sf::Color(120, 200, 120));
    buttonRestartRect.setPosition(120.f, 420.f);
    buttonRestartRect.setOutlineThickness(3);
    buttonRestartRect.setOutlineColor(sf::Color::White);

    // Restart button text
    buttonRestartText.setFont(font);
    buttonRestartText.setString("RESTART");
    buttonRestartText.setCharacterSize(20);
    buttonRestartText.setFillColor(sf::Color::Black);
    buttonRestartText.setPosition(
        buttonRestartRect.getPosition().x + 20.f,
        buttonRestartRect.getPosition().y + 10.f
    );
}

void GameOverScreen::updateStats(int wavesSurvived, int enemiesEliminated, int defensesPlaced) {
    std::string statsStr =
        "Waves survived: " + std::to_string(wavesSurvived) + "\n" +
        "Enemies eliminated: " + std::to_string(enemiesEliminated) + "\n" +
        "Defenses placed: " + std::to_string(defensesPlaced);

    statsText.setString(statsStr);
}

void GameOverScreen::render(sf::RenderWindow& window) const {
    window.draw(overlay);
    window.draw(titleText);
    window.draw(statsText);
    window.draw(buttonRestartRect);
    window.draw(buttonRestartText);
}

bool GameOverScreen::handleClick(const sf::Vector2f& worldPos) const {
    return buttonRestartRect.getGlobalBounds().contains(worldPos);
}
