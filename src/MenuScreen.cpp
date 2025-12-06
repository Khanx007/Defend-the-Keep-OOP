// MenuScreen.cpp
#include "MenuScreen.hpp"
#include <SFML/Graphics.hpp>

void MenuScreen::init(sf::Font& font) {
    // Title
    titleText.setFont(font);
    titleText.setString("DEFEND THE KEEP");
    titleText.setCharacterSize(56);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(120.f, 80.f);

    // Subtitle
    subtitleText.setFont(font);
    subtitleText.setString("Protect the castle from waves of intruders");
    subtitleText.setCharacterSize(20);
    subtitleText.setFillColor(sf::Color(200, 200, 200));
    subtitleText.setPosition(120.f, 150.f);

    // Start button rectangle
    buttonStartRect.setSize({280.f, 60.f});
    buttonStartRect.setFillColor(sf::Color(80, 180, 80));
    buttonStartRect.setPosition(120.f, 240.f);
    buttonStartRect.setOutlineThickness(3);
    buttonStartRect.setOutlineColor(sf::Color::White);

    // Start button text
    buttonStartText.setFont(font);
    buttonStartText.setString("START GAME");
    buttonStartText.setCharacterSize(24);
    buttonStartText.setFillColor(sf::Color::Black);
    buttonStartText.setPosition(
        buttonStartRect.getPosition().x + 20.f,
        buttonStartRect.getPosition().y + 12.f
    );
}

void MenuScreen::render(sf::RenderWindow& window) const {
    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(buttonStartRect);
    window.draw(buttonStartText);
}

bool MenuScreen::handleClick(const sf::Vector2f& worldPos) const {
    return buttonStartRect.getGlobalBounds().contains(worldPos);
}
