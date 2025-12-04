#pragma once
#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Drawable, public sf::Transformable {
public:
    AnimatedSprite() = default;
    void setTexture(const sf::Texture& t, int frameW, int frameH);
    void setFrameTime(float seconds) { frameTime = seconds; }
    void play() { playing = true; }
    void stop() { playing = false; }
    void update(float dt);
    void setFrame(int i);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const sf::Texture* texture = nullptr;
    sf::Sprite sprite;
    int frameW = 0, frameH = 0;
    int cols = 0, rows = 0;
    int curFrame = 0;
    float frameTime = 0.12f;
    float timer = 0.f;
    bool playing = true;
};
