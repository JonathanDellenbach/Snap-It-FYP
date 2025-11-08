#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameState.h"
#include "Menu.h"

class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void processKeys(const std::optional<sf::Event> t_event);
    void processMouseClick(const std::optional<sf::Event> t_event);
    void checkKeyboardState();
    void update(sf::Time t_deltaTime);
    void render();
    bool loadAssets();

    sf::RenderWindow window;
    sf::Font font;
    bool exitGame = false;
    bool m_gameValid = false;

    GameState m_currentState;

    // Menu
    std::unique_ptr<Menu> m_menu;
};

#pragma warning( pop ) 
#endif