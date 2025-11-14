#ifndef NPC_HPP
#define NPC_HPP
#include "Entity.h"

enum class NPCType
{
    Red,
    Blue
};

class NPC : public Entity
{
public:
    NPC(NPCType type, sf::Vector2f startPosition);
    ~NPC();

    void update(sf::Time deltaTime) override;
    void initTexture() override;
    void render(sf::RenderTarget& target) const override;

    NPCType getType() const { return m_type; }

    void setInCameraView(bool inView) { m_isInCameraView = inView; }
    void setPhotographed(bool photographed) { m_hasBeenPhotographed = photographed; }

    //highlight
    void triggerHighlight();
    bool isHighlighted() const { return m_isHighlighted; }

private:
    NPCType m_type;
    bool m_isInCameraView;
    bool m_hasBeenPhotographed;

    //highlight properties
    bool m_isHighlighted = false;
    float m_highlightTimer = 0.0f;
    float HIGHLIGHT_DURATION = 1.0f;
    float NPC_SIZE = 100.0f;

    //highlight overlay
    mutable sf::RectangleShape m_highlightOverlay;
};

#endif