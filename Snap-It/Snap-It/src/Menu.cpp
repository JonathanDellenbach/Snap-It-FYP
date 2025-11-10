#include "Menu.h"
#include <iostream>

Menu::Menu(sf::Font& font, sf::Vector2u windowSize)
    : m_font(font),
    m_titleText(font),
    m_nextState(GameState::Menu),
    m_shouldExit(false),
    m_isValid(true)
{
    //placeholder for the old computer place will replace this with an image/animation later on
    m_computerScreen.setSize(sf::Vector2f(600.0f, 500.0f));
    m_computerScreen.setPosition(sf::Vector2f(
        windowSize.x / 2.0f - 300.0f,
        windowSize.y / 2.0f - 250.0f
    ));
    m_computerScreen.setFillColor(sf::Color(210, 180, 140, 255)); //beige color
    m_computerScreen.setOutlineThickness(5.0f);
    m_computerScreen.setOutlineColor(sf::Color(139, 115, 85)); //darker border

    // Setup title text
    m_titleText.setString("Snap-It");
    m_titleText.setCharacterSize(40);
    m_titleText.setFillColor(sf::Color(50, 50, 50));

    sf::FloatRect titleBounds = m_titleText.getGlobalBounds();
    m_titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f));
    m_titleText.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f - 180.0f));

    setupButtons(windowSize);
}

void Menu::setupButtons(sf::Vector2u windowSize)
{
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;
    float buttonWidth = 250.0f;
    float buttonHeight = 60.0f;
    float buttonSpacing = 80.0f;

    // Play Button
    auto playButton = std::make_unique<Button>(
        "Play",
        sf::Vector2f(centerX - buttonWidth / 2.0f, centerY - buttonHeight / 2.0f - buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_font,
        ButtonID::Play
    );
    m_buttons.push_back(std::move(playButton));

    // About Button
    auto aboutButton = std::make_unique<Button>(
        "About",
        sf::Vector2f(centerX - buttonWidth / 2.0f, centerY - buttonHeight / 2.0f),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_font,
        ButtonID::About
    );
    m_buttons.push_back(std::move(aboutButton));

    // Exit Button
    auto exitButton = std::make_unique<Button>(
        "Exit",
        sf::Vector2f(centerX - buttonWidth / 2.0f, centerY - buttonHeight / 2.0f + buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_font,
        ButtonID::Exit
    );
    m_buttons.push_back(std::move(exitButton));
}

void Menu::handleMouseMove(sf::Vector2f mousePos)
{
    for (auto& button : m_buttons)
    {
        button->update(mousePos);
    }
}

void Menu::handleMouseClick(sf::Vector2f mousePos)
{
    for (auto& button : m_buttons)
    {
        if (button->isClicked(mousePos))
        {
            switch (button->getID())
            {
            case ButtonID::Play:
                m_nextState = GameState::Playing;
                std::cout << "Play button" << std::endl;
                break;

            case ButtonID::About:
                m_nextState = GameState::About;
                std::cout << "About button" << std::endl;
                break;

            case ButtonID::Exit:
                m_shouldExit = true;
                std::cout << "Exit button" << std::endl;
                break;

            case ButtonID::None:
                break;
            }
        }
    }
}

void Menu::update(sf::Time deltaTime) //placeholder for proper images/animations
{
}

void Menu::render(sf::RenderWindow& window)
{
    //for now the template screen
    window.draw(m_computerScreen);

    //title
    window.draw(m_titleText);

    //all buttons
    for (auto& button : m_buttons)
    {
        button->render(window);
    }
}

void Menu::reset()
{
    m_nextState = GameState::Menu;
    m_shouldExit = false;
}