#include "Game.h"
#include <iostream>

Game::Game() :
    window{ sf::VideoMode{ sf::Vector2u{1920U, 1080U}, 32U }, "Snap-It" },
    m_gameValid(false),
    m_currentState(GameState::Menu),
    m_currentPhase(GamePhase::Introduction)
{
    m_gameValid = loadAssets();

    if (m_gameValid)
    {
        m_menu = std::make_unique<Menu>(font, window.getSize());
        if (!m_menu->isValid())
        {
            std::cout << "Failed to load menu!" << std::endl;
            m_gameValid = false;
        }

        //Player
        m_player = std::make_unique<Player>();
        if (!m_player->isValid())
        {
            std::cout << "Failed to load player!" << std::endl;
            m_gameValid = false;
        }

        if (m_player)
        {
            m_player->loadCaptureSound("assets/AUDIO/camera-shutter.wav");
        }

        spawnNPCs();
    }
}

Game::~Game()
{
}

bool Game::loadAssets()
{
    if (!font.openFromFile("assets/fonts/Jersey20-Regular.ttf"))
    {
        std::cout << "Failed to load font!" << std::endl;
        return false;
    }

    std::cout << "All assets loaded successfully!" << std::endl;
    return true;
}

void Game::run()
{
    if (!m_gameValid)
    {
        std::cout << "Game failed to initialize. Exiting..." << std::endl;
        return;
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const float fps{ 60.0f };
    sf::Time timePerFrame = sf::seconds(1.0f / fps);

    while (window.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional newEvent = window.pollEvent())
    {
        if (newEvent->is<sf::Event::Closed>())
        {
            exitGame = true;
        }

        if (newEvent->is<sf::Event::KeyPressed>())
        {
            processKeys(newEvent);
        }

        if (newEvent->is<sf::Event::MouseButtonPressed>())
        {
            processMouseClick(newEvent);
        }

        //mouse movement for button hover effects
        if (newEvent->is<sf::Event::MouseMoved>())
        {
            if (m_currentState == GameState::Menu && m_menu)
            {
                const sf::Event::MouseMoved* mouseMove = newEvent->getIf<sf::Event::MouseMoved>();
                m_menu->handleMouseMove(sf::Vector2f((mouseMove->position.x), (mouseMove->position.y)));
            }
        }
    }
}

void Game::processKeys(const std::optional<sf::Event> t_event)
{
    const sf::Event::KeyPressed* newKeypress = t_event->getIf<sf::Event::KeyPressed>();

    if (sf::Keyboard::Key::Escape == newKeypress->code)
    {
        //if in game return to menu. If in menu exit
        if (m_currentState == GameState::Playing || m_currentState == GameState::Capturing)
        {
            m_currentState = GameState::Menu;
            m_menu->reset();
        }
        else if (m_currentState == GameState::About)
        {
            m_currentState = GameState::Menu;
            m_menu->reset();
        }
        else
        {
            exitGame = true;
        }
    }
}

void Game::processMouseClick(const std::optional<sf::Event> t_event)
{
    const sf::Event::MouseButtonPressed* mouseClick = t_event->getIf<sf::Event::MouseButtonPressed>();

    if (mouseClick->button == sf::Mouse::Button::Left)
    {
        if (m_currentState == GameState::Menu && m_menu) //menu clicks
        {
            m_menu->handleMouseClick(sf::Vector2f((mouseClick->position.x), (mouseClick->position.y)));
        }
        else if (m_currentState == GameState::Playing && m_player) //camera clicks
        {
            m_player->startCapture();
            m_currentState = GameState::Capturing;

            checkNPCsInCamera(); // check what npcs are in camera view
            std::cout << "Gamestate changed: Capturing" << std::endl;
        }
    }
}

void Game::checkKeyboardState()
{
}

void Game::update(sf::Time t_deltaTime)
{
    switch (m_currentState)
    {
    case GameState::Menu:
        if (m_menu)
        {
            m_menu->update(t_deltaTime);

            // Check if menu wants to change state
            if (m_menu->shouldExit())
            {
                exitGame = true; //game closes
            }
            else if (m_menu->getNextState() != GameState::Menu)
            {
                m_currentState = m_menu->getNextState();
                std::cout << "Switching to state: " << static_cast<int>(m_currentState) << std::endl;
            }
        }
        break;

    case GameState::Playing:
        if (m_player)
        {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(mousePixelPos.x, mousePixelPos.y);
            m_player->update(t_deltaTime, mousePos);
        }

        for (auto& npc : m_npcs)
        {
            if (npc && npc->isValid())
            {
                npc->update(t_deltaTime);
            }
        }

        break;

    case GameState::Capturing:
        // Update capture animation
        if (m_player)
        {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(mousePixelPos.x, mousePixelPos.y);
            m_player->update(t_deltaTime, mousePos);

            //check if capture is complete
            if (m_player->captureComplete())
            {
                std::cout << "Capture complete return to Playing gameState" << std::endl;
                m_currentState = GameState::Playing;
                m_player->resetCapture();
            }
        }

        for (auto& npc : m_npcs)
        {
            if (npc && npc->isValid())
            {
                npc->update(t_deltaTime);
            }
        }
        break;

    case GameState::ShowingPhoto:
        // Display captured photo to NPCs
        break;

    case GameState::NPCReaction:
        //NPCs react to photo 
        break;

    case GameState::Paused:
        break;

    case GameState::GameOver:
        break;
    }

    if (exitGame)
    {
        window.close();
    }
}

void Game::render()
{
    window.clear(sf::Color(50, 50, 80)); //Dark bluegray background for my theme

    switch (m_currentState)
    {
    case GameState::Menu:
        if (m_menu)
        {
            m_menu->render(window);
        }
        break;

    case GameState::Playing:
    case GameState::Capturing:

        for (const auto& npc : m_npcs)
        {
            if (npc && npc->isValid())
            {
                npc->render(window);
            }
        }

        if (m_player)
        {
            m_player->render(window);
        }

        {
            sf::Text playText(font);
            playText.setString("PLAYING - Press ESC to return to menu");
            playText.setCharacterSize(40);
            playText.setFillColor(sf::Color::White);
            playText.setPosition(sf::Vector2f(400.0f, 500.0f));
            window.draw(playText);
        }
        break;

    case GameState::About:
    {
        sf::Text aboutText(font);
        aboutText.setString("ABOUT\n\nThis is just template text\n\nPress ESC to return to menu");
        aboutText.setCharacterSize(30);
        aboutText.setFillColor(sf::Color::White);
        aboutText.setPosition(sf::Vector2f(600.0f, 400.0f));
        window.draw(aboutText);
    }
    break;

    case GameState::Paused:
        break;

    case GameState::GameOver:
        break;
    }

    window.display();
}

void Game::spawnNPCs()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    float minX = 100.0f;
    float maxX = 1800.0f;
    float minY = 100.0f;
    float maxY = 900.0f;

    for (int i = 0; i < 10; ++i) //spawn 10 Red NPCs
    {
        float randomX = minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX);
        float randomY = minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY);

        auto redNPC = std::make_unique<NPC>(NPCType::Red, sf::Vector2f(randomX, randomY));
        if (redNPC->isValid())
        {
            m_npcs.push_back(std::move(redNPC)); //ownership from rednpc to npc obj
        }
    }

    
    for (int i = 0; i < 10; ++i) //spawn 10 Blue NPCs
    {
        float randomX = minX + static_cast<float>(std::rand()) / RAND_MAX * (maxX - minX);
        float randomY = minY + static_cast<float>(std::rand()) / RAND_MAX * (maxY - minY);

        auto blueNPC = std::make_unique<NPC>(NPCType::Blue, sf::Vector2f(randomX, randomY));
        if (blueNPC->isValid())
        {
            m_npcs.push_back(std::move(blueNPC)); //ownership from bluenpc to npc obj
        }
    }

    //std::cout << "Total NPCs: " << m_npcs.size() << std::endl;
}

void Game::checkNPCsInCamera()
{
    if (!m_player) return;

    sf::FloatRect cameraArea = m_player->getCaptureArea(); //global bounds of the viewfinder rec
    int capturedCount = 0;

    std::cout << "=== Camera Capture Check ===" << std::endl;

    for (auto& npc : m_npcs)
    {
        if (npc && npc->isValid())
        {
            sf::FloatRect npcBounds = npc->getBounds();

            //check if NPC intersects with camera area
            if (cameraArea.findIntersection(npcBounds).has_value())
            {
                npc->setInCameraView(true);
                npc->setPhotographed(true);
                npc->triggerHighlight();
                capturedCount++;

                std::string npcColor;
                if (npc->getType() == NPCType::Red)
                {
                    npcColor = "Red";
                }
                else if (npc->getType() == NPCType::Blue)
                {
                    npcColor = "Blue";
                }

                //log npc in the capture
                sf::Vector2f npcPos = npc->getPosition();
                std::cout << "Captured " << npcColor << " NPC at the pos ("<< npcPos.x << ", " << npcPos.y << ")" << std::endl;
            }
            else
            {
                npc->setInCameraView(false);
            }
        }
    }

    std::cout << "Total NPCs captured: " << capturedCount << std::endl;
    std::cout << "=========================" << std::endl;
}