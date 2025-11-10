#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Button.h"
#include "GameState.h"

class Menu
{
public:
    Menu(sf::Font& font, sf::Vector2u windowSize);

    void handleMouseMove(sf::Vector2f mousePos);
    void handleMouseClick(sf::Vector2f mousePos);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    GameState getNextState() const { return m_nextState; }
    bool shouldExit() const { return m_shouldExit; }
    void reset(); //reset menu state when returning modifying the gamestate

    bool isValid() const { return m_isValid; }

private:
    void setupButtons(sf::Vector2u windowSize);

    std::vector<std::unique_ptr<Button>> m_buttons;
    sf::Font& m_font;

    sf::RectangleShape m_computerScreen; //template computer screen
    sf::Text m_titleText;

    GameState m_nextState;
    bool m_shouldExit;
    bool m_isValid;
};

#endif