#include "NPC.h"
#include <iostream>

NPC::NPC(NPCType type, sf::Vector2f startPosition)
    : m_type(type)
    , m_isHighlighted(false)
    , m_highlightTimer(0.0f)
{
    setPosition(startPosition);
    initTexture();

    //setup highlight overlay
    m_highlightOverlay.setSize(sf::Vector2f(NPC_SIZE, NPC_SIZE));
    m_highlightOverlay.setFillColor(sf::Color(255, 255, 0, 150));
    m_highlightOverlay.setOutlineThickness(2.0f);
    m_highlightOverlay.setOutlineColor(sf::Color::Yellow);

    if (!isValid())
    {
        std::cout << "NPC failed to init" << std::endl;
    }
}

NPC::~NPC()
{
}

void NPC::initTexture()
{
    std::string texturePath;

    if (m_type == NPCType::Red)
    {
        texturePath = "assets/IMAGES/redSquare.png";
    }
    else if (m_type == NPCType::Blue)
    {
        texturePath = "assets/IMAGES/blueSquare.png";
    }

    bool textureLoaded = m_texture.loadFromFile(texturePath);

    if (textureLoaded)
    {
        m_sprite.setTexture(m_texture, true);

        //mark this NPC valid
        m_isValid = true;

        std::string npcColor;
        if (m_type == NPCType::Red)
        {
            npcColor = "Red";
        }
        else if (m_type == NPCType::Blue)
        {
            npcColor = "Blue";
        }

        //std::cout << "NPC created: " << npcColor << " at position (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;
    }
    else
    {
        std::cout << "failed to load NPC texture" << std::endl;
        m_isValid = false;
    }
}

void NPC::update(sf::Time deltaTime)
{
    if (m_isHighlighted)
    {
        m_highlightTimer += deltaTime.asSeconds();

        if (m_highlightTimer >= HIGHLIGHT_DURATION)
        {
            m_isHighlighted = false;
            m_highlightTimer = 0.0f;
            //std::cout << "NPC unhighlighted at position (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;
        }
    }
}

void NPC::render(sf::RenderTarget& target) const
{
    if (m_isValid)
    {
        target.draw(m_sprite);

        if (m_isHighlighted) //draw highlight
        {
            m_highlightOverlay.setPosition(getPosition());
            target.draw(m_highlightOverlay);
        }
    }
}

void NPC::triggerHighlight()
{
    m_isHighlighted = true;
    m_highlightTimer = 0.0f;
    std::string npcColor;
    if (m_type == NPCType::Red)
    {
        npcColor = "Red";
    }
    else if (m_type == NPCType::Blue)
    {
        npcColor = "Blue";
    }

    //std::cout << "NPC highlighted: " << npcColor << " at position (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;
}