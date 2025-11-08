#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class ButtonID
{
    Play,
    About,
    Exit,
    None
};

class Button
{
public:
    Button(const std::string& text, sf::Vector2f position, sf::Vector2f size, sf::Font& font, ButtonID id);

    void render(sf::RenderWindow& window);
    void update(sf::Vector2f mousePos);
    bool isClicked(sf::Vector2f mousePos);

    ButtonID getID() const { return m_id; }
    bool isValid() const { return m_isValid; }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_clickColor;
    bool m_isHovered;
    bool m_isValid;
    ButtonID m_id;
};

#endif