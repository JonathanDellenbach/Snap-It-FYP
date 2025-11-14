#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<vector>
#include "GameState.h"
#include "Menu.h"
#include "Player.h"
#include "NPC.h"

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

    void spawnNPCs();
    void checkNPCsInCamera();

    sf::RenderWindow window;
    sf::Font font;
    bool exitGame = false;
    bool m_gameValid = false;

    //State management
    GameState m_currentState;

    //Phase Management
    GamePhase m_currentPhase;

    // Menu
    std::unique_ptr<Menu> m_menu;

    //Player
    std::unique_ptr<Player> m_player;

    //npc
    std::vector<std::unique_ptr<NPC>> m_npcs;
};

#pragma warning( pop ) 
#endif