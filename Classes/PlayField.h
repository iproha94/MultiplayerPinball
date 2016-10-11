#pragma once

#include "cocos2d.h"
#include "PhysicsCategory.h"
#include "MyBodyParser.h"
#include <string>
#include <sstream>
#include <Builder.h>

using namespace std;

USING_NS_CC;

//for android
template <typename T>
std::string to_string(T value) {
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

class PlayField : public Builder {
//Здесь переопределены все методы класса Builder
public:
    PlayField();
    void createBoundary(Node *_layer);
    void createFlippers(Node *Father, int Side, int Player);
    void createObsticles(Node *Father);
    void createBumper(Node *Father, const Vec2 &position, std::string name);
    void createKicker(Node *Father, const Vec2 &position, float rotation);
    void createSlingshot(Node *Father, int Side, int Player);
    void createTripleBumper(Node *Father, int Player);
    void createWall(Node *Father, int Player);
    void createPlunger(Node *Father, int Player);

    Size getVisiblesize() { return visibleSize; }
    std::string Name_generator(std::string base, int side, int player = 0);


    CREATE_FUNC(PlayField)
    
protected:
    Size visibleSize;
    int localOffset;
    int Offset;
    int flipperLength;
    float shrinker;
    Vec2 Origin;
};
