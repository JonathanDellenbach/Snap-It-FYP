#include "Button.h"

Button::Button(const std::string& text, sf::Vector2f position, sf::Vector2f size, sf::Font& font, ButtonID id)
    : m_isHovered(false), m_isValid(true), m_text(font), m_id(id)
{
    // Setup button shape
    m_shape.setSize(size);
    m_shape.setPosition(position);

    //colors
    m_normalColor = sf::Color(100, 100, 100, 200);
    m_hoverColor = sf::Color(150, 150, 150, 220);
    m_clickColor = sf::Color(80, 80, 80, 200);

    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineThickness(3.0f);
    m_shape.setOutlineColor(sf::Color::White);

    //text
    m_text.setString(text);
    m_text.setCharacterSize(30);
    m_text.setFillColor(sf::Color::White);

    //center text in button
    sf::FloatRect textBounds = m_text.getGlobalBounds();
    m_text.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
    m_text.setPosition(sf::Vector2f(
        position.x + size.x / 2.0f,
        position.y + size.y / 3.0f
    ));
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_text);
}

void Button::update(sf::Vector2f mousePos)
{
    //check if mouse is over button
    if (m_shape.getGlobalBounds().contains(mousePos))
    {
        m_isHovered = true;
        m_shape.setFillColor(m_hoverColor);
    }
    else
    {
        m_isHovered = false;
        m_shape.setFillColor(m_normalColor);
    }
}

bool Button::isClicked(sf::Vector2f mousePos)
{
    if (m_shape.getGlobalBounds().contains(mousePos))
    {
        m_shape.setFillColor(m_clickColor);
        return true;  // Just return true, Menu will check the ID
    }
    return false;
}