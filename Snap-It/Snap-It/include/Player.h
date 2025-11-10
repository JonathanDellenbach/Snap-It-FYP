#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player
{
public:
    Player();
    ~Player() = default;

    void update(sf::Time deltaTime, sf::Vector2f mousePos);
    void render(sf::RenderWindow& window);

    void startCapture(); //when LMB is pressed
    bool isCapturing() { return m_isCapturing; }
    bool captureComplete() { return m_captureComplete; }
    void resetCapture();  //reset once done to capture again

    sf::FloatRect getCaptureArea() const; //returns camera bounds for NPC detection
    bool isValid() { return m_isValid; }

    bool loadCaptureSound(const std::string& filepath); //load the shutter sound effect

private:
    //camera frame
    sf::RectangleShape m_viewfinder;
    sf::Vector2f m_mousePosition;

    // shutter animation (might be replaced by sprites)
    bool m_isCapturing;
    bool m_captureComplete;
    float m_captureTimer;
    sf::RectangleShape m_flashOverlay;

    // Audio
    sf::SoundBuffer m_captureSoundBuffer;
    std::unique_ptr<sf::Sound> m_captureSound;
    bool m_soundLoaded;

    const float VIEWFINDER_WIDTH = 400.0f;
    const float VIEWFINDER_HEIGHT = 200.0f;
    const float CAPTURE_DURATION = 0.2f; // 0.2 seconds for flash

    bool m_isValid;
};

#endif