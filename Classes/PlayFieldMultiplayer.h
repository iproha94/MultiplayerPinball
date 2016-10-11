#pragma once
#include "PlayField.h"

class PlayFieldMultiplayer : public PlayField {
public:
    void createPlunger(cocos2d::Node *Father, int Player);
    void createWall(cocos2d::Node *Father, int Player);
    void createObsticles(cocos2d::Node *Father) {
        auto size = getVisiblesize();

        //creating static border between players
        auto border = Sprite::create("Border.png");
        border->setPosition(Vec2(0 , size.height / 7));
        border->setName("StaticBorder");

        auto BorderSize = border->getContentSize();

        auto borderBody = cocos2d::PhysicsBody::createBox(BorderSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
        borderBody->setDynamic(false);
        border->setPhysicsBody(borderBody);
        Father->addChild(border, 10);
    }
};

void PlayFieldMultiplayer::createPlunger(cocos2d::Node *Father, int Player) {
    auto size = visibleSize;
    auto Plunger = cocos2d::Sprite::create("plunger.png");
    const int defaultPosition = size.height / 3;
    Plunger->setPosition(Player * (size.width * 0.45 + Plunger->getContentSize().width * 0.6), defaultPosition * 0.75);
    Plunger->setName(Name_generator("Plunger", Player));
    Father->addChild(Plunger, 100);

    auto PlungerStatic = cocos2d::Sprite::create("plunger.png");
    PlungerStatic->setPosition(Player * (size.width * 0.45 + Plunger->getContentSize().width * 0.6), -defaultPosition);
    PlungerStatic->setName("PlungerStatic");
    Father->addChild(PlungerStatic, 100);

    auto anchor1 = cocos2d::Vec2(Plunger->getAnchorPoint().x - 13, Plunger->getAnchorPoint().y);
    auto anchor2 = cocos2d::Vec2(PlungerStatic->getAnchorPoint().x - 13, PlungerStatic->getAnchorPoint().y);

    auto anchor11 = cocos2d::Vec2(Plunger->getAnchorPoint().x + 13, Plunger->getAnchorPoint().y);
    auto anchor22 = cocos2d::Vec2(PlungerStatic->getAnchorPoint().x + 13, PlungerStatic->getAnchorPoint().y);

    if(Player == -1) {
        auto PlungerBody = cocos2d::PhysicsBody::createBox(Plunger->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.2f));
        PlungerBody->setDynamic(true);
        Plunger->setPhysicsBody(PlungerBody);
        auto PlungerStaticBody = cocos2d::PhysicsBody::createBox(Plunger->getContentSize(), cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
        PlungerStaticBody->setDynamic(false);
        PlungerStatic->setPhysicsBody(PlungerStaticBody);
        auto leftSpring = cocos2d::PhysicsJointSpring::construct(PlungerBody, PlungerStaticBody, anchor1, anchor2, 1000, 50);
        auto rightSpring = cocos2d::PhysicsJointSpring::construct(PlungerBody, PlungerStaticBody, anchor11, anchor22, 1000, 50);
        Father->getScene()->getPhysicsWorld()->addJoint(leftSpring);
        Father->getScene()->getPhysicsWorld()->addJoint(rightSpring);
    }

    auto StringLength = Plunger->getPosition().y - PlungerStatic->getPosition().y;
    auto StringSegment = cocos2d::Sprite::create("SpringSegment.png");

    int SegmentHeight = StringSegment->getContentSize().width / 1.414;
    int SegmentsNumber = StringLength / SegmentHeight;
    auto defaultStringPosition = cocos2d::Vec2(PlungerStatic->getPosition().x, PlungerStatic->getPosition().y + PlungerStatic->getContentSize().height);
    const int alpha = 45;
    int multiplier = 1;

    for (int i = 0; i < SegmentsNumber - 1; ++i) {
        auto StringSegment1 = cocos2d::Sprite::create("SpringSegment.png");
        StringSegment1->setName(Name_generator("Segment", Player) + to_string(i + 1));

        StringSegment1->setPosition(defaultStringPosition.x, defaultStringPosition.y + i * SegmentHeight);
        multiplier *= -1;
        StringSegment1->setRotation(alpha * multiplier);
        Father->addChild(StringSegment1);
    }
}

void PlayFieldMultiplayer::createWall(cocos2d::Node *Father, int Player) {
    auto size = visibleSize;
    auto Wall = cocos2d::Sprite::create("Wall.png");
    Wall->setPosition(cocos2d::Vec2(Player * size.width * 0.45, 28));
    Wall->setName("StaticWall");

    auto WallSize = Wall->getContentSize();

    if(Player == -1) {
        auto SideWallBody = cocos2d::PhysicsBody::createBox(WallSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 0.0f));
        SideWallBody->setDynamic(false);
        Wall->setPhysicsBody(SideWallBody);
    }


    Father->addChild(Wall, 1000);

    auto WallPosition = Wall->getPosition();

    std::string TubeNameJson;

    TubeNameJson = Name_generator("Tube", Player);

    auto InsideTubeWall = cocos2d::Sprite::create(Name_generator("Tube.png", Player));
    InsideTubeWall->setPosition(WallPosition.x - Player * InsideTubeWall->getContentSize().width / 2, WallPosition.y + Wall->getContentSize().height / 2 + InsideTubeWall->getContentSize().height / 2 /*временный костыль*/);

    MyBodyParser::getInstance()->parseJsonFile(TubeNameJson);

    if (Player == -1) {
        auto WallBody = MyBodyParser::getInstance()->bodyFormJson(InsideTubeWall, TubeNameJson, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.2f));
        WallBody->setDynamic(false);
        InsideTubeWall->setPhysicsBody(WallBody);
    }

    Father->addChild(InsideTubeWall);

    std::string BigTubeNameJson;

    BigTubeNameJson = Name_generator("BigTube", Player);

    auto InsideTubeBigWall = cocos2d::Sprite::create(Name_generator("BigTube.png", Player));
    InsideTubeBigWall->setPosition(Player * (size.width / 2 - InsideTubeBigWall->getContentSize().width
                                    / 2), WallPosition.y + Wall->getContentSize().height / 2 + InsideTubeBigWall->getContentSize().height / 2/*временный костыль*/);

    MyBodyParser::getInstance()->parseJsonFile(BigTubeNameJson);

    if (Player == -1) {
        auto BigWallBody = MyBodyParser::getInstance()->bodyFormJson(InsideTubeBigWall, BigTubeNameJson, PhysicsMaterial(0.0f, 0.0f, 0.2f));
        BigWallBody->setDynamic(false);
        InsideTubeBigWall->setPhysicsBody(BigWallBody);
    }

    Father->addChild(InsideTubeBigWall);
}
