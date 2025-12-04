#include "AnimatedSprite.hpp"

void AnimatedSprite::setTexture(const sf::Texture& t, int fw, int fh) {
    texture = &t;
    frameW = fw; frameH = fh;
    cols = t.getSize().x / frameW;
    rows = t.getSize().y / frameH;
    sprite.setTexture(*texture);
    sprite.setOrigin(frameW/2.f, frameH/2.f);
    setFrame(0);
}

void AnimatedSprite::update(float dt) {
    if (!playing || !texture) return;
    timer += dt;
    if (timer >= frameTime) {
        timer -= frameTime;
        curFrame = (curFrame + 1) % (cols * rows);
        setFrame(curFrame);
    }
}

void AnimatedSprite::setFrame(int i) {
    curFrame = i;
    int cx = curFrame % cols;
    int cy = curFrame / cols;
    sprite.setTextureRect(sf::IntRect(cx*frameW, cy*frameH, frameW, frameH));
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!texture) return;
    states.transform *= getTransform();
    target.draw(sprite, states);
}
 