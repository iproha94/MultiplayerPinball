#include "PlayField.h"
#define MiddleChild 10
#define LowerChild 5

PlayField::PlayField() {
    shrinker = 0.9;
    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    Origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    Offset = (int) (visibleSize.width / 5);
    localOffset = (int) (visibleSize.width * shrinker);
}

void PlayField::createBoundary(cocos2d::Node *_layer) {
    auto size = visibleSize;
    size.height += Offset;
    auto edgeBody = cocos2d::PhysicsBody::createEdgeBox( size, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f), 3);
    _layer->setPhysicsBody(edgeBody);
}

void PlayField::createFlippers(cocos2d::Node *Father, int Side, int Player) {

    const int FlipperAngle = 30;

    auto size = visibleSize;
    size.width *= shrinker;
    auto flipper = cocos2d::Sprite::create(Name_generator("Flipper.png", Side));
    auto companion = cocos2d::Sprite::create(Name_generator("Companion.png", Side));
    flipper->setPosition(cocos2d::Vec2(Offset * Side / 2 + Player * localOffset / 4, -size.height / 4));
    companion->setPosition(cocos2d::Vec2(((size.width / 2 - Offset) / 2 + Offset) * Side / 2 + Player * localOffset / 4,
                                         (float) (-size.height / 5.3)));
    flipper->setName(Name_generator("Flipper", Side, Player));

    Father->addChild(flipper, MiddleChild);
    Father->addChild(companion, MiddleChild);

    auto FlipperSize = flipper->getContentSize();
    auto CompanionSize = companion->getContentSize();

    auto flipperBody = cocos2d::PhysicsBody::createBox(FlipperSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
    auto companionBody = cocos2d::PhysicsBody::createBox(CompanionSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));

    companionBody->setRotationOffset((-Side) * FlipperAngle);
    flipperBody->setRotationOffset((-Side) * FlipperAngle);

    flipperBody->setDynamic(true);
    companionBody->setDynamic(false);

    //flipperBody->getShape(PhysicsShapeBox)->
    //flipperBody->setPositionOffset(Vec2(-Side * size.width / 14, 0));

    companion->setRotation((-Side) * FlipperAngle);
    flipper->setRotation((-Side) * FlipperAngle);

    flipper->setPhysicsBody(flipperBody);
    companion->setPhysicsBody(companionBody);

    auto PinJoint = cocos2d::PhysicsJointPin::construct(flipper->getPhysicsBody(), Father->getPhysicsBody(), cocos2d::Vec2(visibleSize.width / 2 + Side * Offset / 2 + Player * localOffset / 4, size.height / 4 - Offset / 2));

    auto LimitJoint = cocos2d::PhysicsJointRotaryLimit::construct( companion->getPhysicsBody(), flipperBody, 0, 1.2);
    if (Side + 1) {
        LimitJoint = cocos2d::PhysicsJointRotaryLimit::construct(flipperBody, companion->getPhysicsBody(), 0, 1.2);
    }

    PinJoint->setCollisionEnable(false);
    LimitJoint->setCollisionEnable(false);

    Father->getScene()->getPhysicsWorld()->addJoint(PinJoint);
    Father->getScene()->getPhysicsWorld()->addJoint(LimitJoint);
}

void PlayField::createObsticles(cocos2d::Node *Father) {
    auto size = visibleSize;

    //creating static border between players
    auto border = cocos2d::Sprite::create("Border.png");
    border->setPosition(cocos2d::Vec2(0 , size.height / 7));
    border->setName("StaticBorder");

    auto BorderSize = border->getContentSize();

    auto borderBody = cocos2d::PhysicsBody::createBox(BorderSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
    borderBody->setDynamic(false);
    border->setPhysicsBody(borderBody);
    Father->addChild(border, MiddleChild);
}

void PlayField::createKicker(cocos2d::Node *Father, const cocos2d::Vec2 &position, float rotation) {
    auto kicker = cocos2d::Sprite::create("flipper.png");
    kicker->setName("Kicker");
    kicker->setScale(0.41);

    auto body = cocos2d::PhysicsBody::createBox(kicker->getContentSize() * kicker->getScale());
    body->setCategoryBitmask((int)PhysicsCategory::Kicker);
    body->setContactTestBitmask((int)PhysicsCategory::Ball);
    body->setDynamic(false);

    body->setRotationOffset(rotation);
    kicker->setRotation(rotation);
    kicker->setPhysicsBody(body);
    kicker->setPosition(position);
    Father->addChild(kicker, MiddleChild);
}

void PlayField::createBumper(cocos2d::Node *Father, const cocos2d::Vec2 &position, std::string name) {
    auto bumper = cocos2d::Sprite::create(name);
    bumper->setName("Bumper");
    bumper->setScale(0.25);

    auto body = cocos2d::PhysicsBody::createCircle(bumper->getContentSize().width / 9);
    body->setDynamic(false);
    body->setCategoryBitmask((int)PhysicsCategory::Bumper);
    body->setContactTestBitmask((int)PhysicsCategory::Ball);
    bumper->setPhysicsBody(body);

    bumper->setPosition(position);
    Father->addChild(bumper, MiddleChild);
}

void PlayField::createSlingshot(cocos2d::Node *Father, int Side, int Player) {
    auto size = visibleSize;
    size.width *= shrinker;
    auto SlingshotPosition = cocos2d::Vec2((float) (size.width * 0.16 * Side + Player * localOffset / 4), 0);
    createKicker(Father, SlingshotPosition, (-Side) * 65);

    auto horizontalWall = cocos2d::Sprite::create("flipper.png");
    horizontalWall->setScale(0.20);

    auto horizontalWallSize = cocos2d::Size(horizontalWall->getContentSize().width * horizontalWall->getScale(), horizontalWall->getContentSize().height * horizontalWall->getScale());
    horizontalWall->setPosition(SlingshotPosition.x, (float) (-horizontalWallSize.width / 0.6 / 2));
    auto HorizontalBody = cocos2d::PhysicsBody::createBox(horizontalWallSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
    HorizontalBody->setDynamic(false);

    HorizontalBody->setRotationOffset((-Side) * 30);
    horizontalWall->setRotation((-Side) * 30);

    horizontalWall->setPhysicsBody(HorizontalBody);

    Father->addChild(horizontalWall, MiddleChild);

    auto verticalWall = cocos2d::Sprite::create("flipper.png");
    verticalWall->setPosition((float) (SlingshotPosition.x + Side * verticalWall->getContentSize().width * 0.45 / 4),
                              (float) (verticalWall->getContentSize().height * 1.5));
    verticalWall->setScale(0.37);

    auto verticalBody = cocos2d::PhysicsBody::createBox(verticalWall->getContentSize() * verticalWall->getScale());
    verticalBody->setDynamic(false);

    verticalBody->setRotationOffset(90);
    verticalWall->setRotation(90);

    verticalWall->setPhysicsBody(verticalBody);

    Father->addChild(verticalWall, MiddleChild);

    auto slingshot = cocos2d::Sprite::create(Name_generator("Slingshot.png", Side));
    slingshot->setPosition(SlingshotPosition.x + Side * 7, SlingshotPosition.y);
    Father->addChild(slingshot, MiddleChild);
}

void PlayField::createTripleBumper(cocos2d::Node *Father, int Player) {
    auto size = visibleSize;
    size.width *= shrinker;
    size.height += Offset;
    createBumper(Father, Vec2( localOffset / 5 * Player, size.height / 4), "bumperI.png");
    createBumper(Father, Vec2( localOffset / 10 * 3 * Player, size.height / 4), "bumperP.png");
    createBumper(Father, Vec2( localOffset / 4 * Player, size.height / 6), "bumperE.png");
}

void PlayField::createWall(cocos2d::Node *Father, int Player) {
    auto size = visibleSize;
    auto Wall = cocos2d::Sprite::create("Wall.png");
    Wall->setPosition(cocos2d::Vec2((float) (Player * size.width * 0.45), 28));
    Wall->setName("StaticWall");

    auto WallSize = Wall->getContentSize();

    auto SideWallBody = cocos2d::PhysicsBody::createBox(WallSize, cocos2d::PhysicsMaterial(0.2f, 0.0f, 0.0f));
    SideWallBody->setDynamic(false);
    Wall->setPhysicsBody(SideWallBody);
    Father->addChild(Wall, MiddleChild);

    auto WallPosition = Wall->getPosition();

    std::string TubeNameJson;

    TubeNameJson = Name_generator("Tube", Player);

    auto InsideTubeWall = cocos2d::Sprite::create(Name_generator("Tube.png", Player));
    InsideTubeWall->setPosition(WallPosition.x - Player * InsideTubeWall->getContentSize().width / 2, WallPosition.y + Wall->getContentSize().height / 2 + InsideTubeWall->getContentSize().height / 2 /*временный костыль*/);

    MyBodyParser::getInstance()->parseJsonFile(TubeNameJson);

    auto WallBody = MyBodyParser::getInstance()->bodyFormJson(InsideTubeWall, TubeNameJson, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.2f));
    WallBody->setDynamic(false);
    InsideTubeWall->setPhysicsBody(WallBody);

    Father->addChild(InsideTubeWall, MiddleChild);

    std::string BigTubeNameJson;

    BigTubeNameJson = Name_generator("BigTube", Player);

    auto InsideTubeBigWall = cocos2d::Sprite::create(Name_generator("BigTube.png", Player));
    InsideTubeBigWall->setPosition(Player * (size.width / 2 - InsideTubeBigWall->getContentSize().width
                                    / 2), WallPosition.y + Wall->getContentSize().height / 2 + InsideTubeBigWall->getContentSize().height / 2/*временный костыль*/);

    MyBodyParser::getInstance()->parseJsonFile(BigTubeNameJson);

    auto BigWallBody = MyBodyParser::getInstance()->bodyFormJson(InsideTubeBigWall, BigTubeNameJson, cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.2f));
    BigWallBody->setDynamic(false);
    InsideTubeBigWall->setPhysicsBody(BigWallBody);

    Father->addChild(InsideTubeBigWall, MiddleChild);

}

void PlayField::createPlunger(cocos2d::Node *Father, int Player) {
    auto size = visibleSize;
    auto Plunger = cocos2d::Sprite::create("plunger.png");
    const int defaultPosition = (const int) (size.height / 3);
    Plunger->setPosition((float) (Player * (size.width * 0.45 + Plunger->getContentSize().width * 0.6)),
                         (float) (defaultPosition * 0.75));
    Plunger->setName(Name_generator("Plunger", Player));
    Father->addChild(Plunger, MiddleChild);

    auto PlungerBody = cocos2d::PhysicsBody::createBox(Plunger->getContentSize(),
                                                       cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.2f));
    PlungerBody->setDynamic(true);
    Plunger->setPhysicsBody(PlungerBody);

    auto PlungerStatic = cocos2d::Sprite::create("plunger.png");
    PlungerStatic->setPosition((float) (Player * (size.width * 0.45 + Plunger->getContentSize().width * 0.6)),
                               -defaultPosition);
    PlungerStatic->setName("PlungerStatic");
    Father->addChild(PlungerStatic, MiddleChild);

    auto PlungerStaticBody = cocos2d::PhysicsBody::createBox(Plunger->getContentSize(),
                                                             cocos2d::PhysicsMaterial(0.2f, 0.0f, 1.0f));
    PlungerStaticBody->setDynamic(false);
    PlungerStatic->setPhysicsBody(PlungerStaticBody);

    auto anchor1 = cocos2d::Vec2(Plunger->getAnchorPoint().x - 13, Plunger->getAnchorPoint().y);
    auto anchor2 = cocos2d::Vec2(PlungerStatic->getAnchorPoint().x - 13, PlungerStatic->getAnchorPoint().y);

    auto leftSpring = cocos2d::PhysicsJointSpring::construct(PlungerBody, PlungerStaticBody,
                                                             anchor1, anchor2, 1000, 50);

    anchor1 = cocos2d::Vec2(Plunger->getAnchorPoint().x + 13, Plunger->getAnchorPoint().y);
    anchor2 = cocos2d::Vec2(PlungerStatic->getAnchorPoint().x + 13, PlungerStatic->getAnchorPoint().y);

    auto rightSpring = cocos2d::PhysicsJointSpring::construct(PlungerBody, PlungerStaticBody,
                                                              anchor1, anchor2, 1000, 50);
    Father->getScene()->getPhysicsWorld()->addJoint(leftSpring);
    Father->getScene()->getPhysicsWorld()->addJoint(rightSpring);

    auto StringLength = Plunger->getPosition().y - PlungerStatic->getPosition().y;
    auto StringSegment = cocos2d::Sprite::create("SpringSegment.png");

    int SegmentHeight = (int) (StringSegment->getContentSize().width / 1.414);
    int SegmentsNumber = (int) (StringLength / SegmentHeight);
    auto defaultStringPosition = cocos2d::Vec2(PlungerStatic->getPosition().x,
                                               PlungerStatic->getPosition().y + PlungerStatic->getContentSize().height);
    const int alpha = 45;
    int multiplier = 1;

    for (int i = 0; i < SegmentsNumber - 1; ++i) {
        auto StringSegment1 = cocos2d::Sprite::create("SpringSegment.png");
        StringSegment1->setName(Name_generator("Segment", Player) + to_string(i + 1));

        StringSegment1->setPosition(defaultStringPosition.x, defaultStringPosition.y + i * SegmentHeight);
        multiplier *= -1;
        StringSegment1->setRotation(alpha * multiplier);
        Father->addChild(StringSegment1, LowerChild);
    }
}

std::string PlayField::Name_generator(std::string base, int side, int player) {
    std::string result = "";
    if (player == -1) {
        result = "Player1";
    }
    if (player == 1) {
        result = "Player2";
    }
    if (side == -1) {
        result += "Left";
    }
    if (side == 1) {
        result += "Right";
    }
    result += base;
    return result;
}
