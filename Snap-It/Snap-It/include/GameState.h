#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

enum class GameState
{
    Menu,           // Main menu screen
    Playing,        // Gameplay
    Capturing,      // Camera shutter
    ShowingPhoto,   // Display the captured photo to NPCs
    NPCReaction,    // NPCs react to the photo shown
    Paused,         // Game paused
    GameOver,       // End state
    About           // About
};

enum class GamePhase
{
    Introduction,   // Initial peaceful state
    FirstCapture,   // After first photo
    Tension,        // Things start getting weird
    Escalation,     // NPCs becoming more extreme
    Chaos,          // Full breakdown
    Ending          // Final sequence
};

#endif