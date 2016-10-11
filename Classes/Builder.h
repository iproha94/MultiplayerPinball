#pragma once

class Builder : public cocos2d::Layer {
public:
    virtual void createBoundary(Node *_layer) {}
    virtual void createFlippers(Node *Father, int Side, int Player) {}
    virtual void createObsticles(Node *Father) {}
    virtual void createBumper(Node *Father, const Vec2 &position, std::string name) {}
    virtual void createKicker(Node *Father, const Vec2 &position, float rotation) {}
    virtual void createSlingshot(Node *Father, int Side, int Player) {}
    virtual void createTripleBumper(Node *Father, int Player) {}
    virtual void createWall(Node *Father, int Player) {}
    virtual void createPlunger(Node *Father, int Player) {}
    //Методы не объявляются чисто виртуальными, чтобы можно было замещать часть методов
protected:
    Builder() {}
};
