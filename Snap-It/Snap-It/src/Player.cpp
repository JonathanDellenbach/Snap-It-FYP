#include "Player.h"
#include <iostream>

Player::Player()
    : m_mousePosition(0.0f, 0.0f)
    , m_isCapturing(false)
    , m_captureComplete(false)
    , m_captureTimer(0.0f)
    , m_soundLoaded(false)
    , m_isValid(true)
{
    // Setup camera frame
    m_viewfinder.setSize(sf::Vector2f(VIEWFINDER_WIDTH, VIEWFINDER_HEIGHT));
    m_viewfinder.setFillColor(sf::Color(255, 255, 255, 0));
    m_viewfinder.setOutlineThickness(3.0f);
    m_viewfinder.setOutlineColor(sf::Color::White);

    //Set origin to center on mouse
    m_viewfinder.setOrigin(sf::Vector2f(VIEWFINDER_WIDTH / 2.0f, VIEWFINDER_HEIGHT / 2.0f));

    // flash overlay
    m_flashOverlay.setSize(sf::Vector2f(VIEWFINDER_WIDTH, VIEWFINDER_HEIGHT));
    m_flashOverlay.setOrigin(sf::Vector2f(VIEWFINDER_WIDTH / 2.0f, VIEWFINDER_HEIGHT / 2.0f));
    m_flashOverlay.setFillColor(sf::Color(255, 255, 255, 0));

    std::cout << "Camera init" << std::endl;
}

void Player::update(sf::Time deltaTime, sf::Vector2f mousePos)
{
    //update mouse position
    m_mousePosition = mousePos;

    //center on mouse position
    m_viewfinder.setPosition(m_mousePosition);

    //center flash overlay on mouse position
    m_flashOverlay.setPosition(m_mousePosition);

    //capture animation
    if (m_isCapturing)
    {
        m_captureTimer += deltaTime.asSeconds();

        //calculate flash fade
        float progress = m_captureTimer / CAPTURE_DURATION;
        if (progress <= 1.0f)
        {
            // Fade from white to transparent
            int alpha = static_cast<int>(255 * (1.0f - progress));
            m_flashOverlay.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        else //capture animation complete
        {
            m_isCapturing = false;
            m_captureComplete = true;
            m_captureTimer = 0.0f;
            m_flashOverlay.setFillColor(sf::Color(255, 255, 255, 0));
            std::cout << "Capture finish" << std::endl;
        }
    }
}

void Player::render(sf::RenderWindow& window)
{
    //draw camera box
    window.draw(m_viewfinder);

    //draw flash overlay if capturing
    if (m_isCapturing)
    {
        window.draw(m_flashOverlay);
    }
}

void Player::startCapture()
{
    if (!m_isCapturing)
    {
        m_isCapturing = true;
        m_captureComplete = false;
        m_captureTimer = 0.0f;
        m_flashOverlay.setFillColor(sf::Color(255, 255, 255, 255));

        //play capture sound
        if (m_soundLoaded && m_captureSound)
        {
            m_captureSound->play();
        }

        std::cout << "Capture start" << std::endl;
    }
}

void Player::resetCapture()
{
    m_captureComplete = false;
}

sf::FloatRect Player::getCaptureArea() const
{
    return m_viewfinder.getGlobalBounds();
}

bool Player::loadCaptureSound(const std::string& filepath)
{
    if (m_captureSoundBuffer.loadFromFile(filepath))
    {
        //create the sound object
        m_captureSound = std::make_unique<sf::Sound>(m_captureSoundBuffer);
        m_soundLoaded = true;
        return true;
    }
    else
    {
        std::cout << "Failed to load camera sound" << std::endl;
        m_soundLoaded = false;
        return false;
    }
}