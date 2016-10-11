#include "GameScene.h"
#include "MyBodyParser.h"
#include "pausescene.h"
#include "gameoverscene.h"
#include "PlayFieldMultiplayer.h"
#include "SimpleAudioEngine.h"
#include "Export.h"

using namespace extension;
using namespace CocosDenshion;

cocos2d::Scene* GameScene::createScene(int _GameMode, string ip, string name) {
    const int DefaultHeight = 640;
    const int GravityVector = -350;
    auto size = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Director::getInstance()->setContentScaleFactor((float) (1.0 / size.height * DefaultHeight));
    auto scene = cocos2d::Scene::createWithPhysics();

    scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, GravityVector));
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setAutoStep(false);

    auto layer = GameScene::create();
    size.height += size.width / 5;
    layer->setPosition(size.width / 2, size.height / 2 - size.width / 5);
    scene->addChild(layer);

    layer->setGameMode(_GameMode);

    Builder* BuilderPtr;

    if (!_GameMode) {
        BuilderPtr = new PlayField;
    } else {
        BuilderPtr = new PlayField;
    }

    layer->GetPlayField(BuilderPtr);

    layer->createBallForPlayer(layer->player1->getSide());
    layer->player1->add_ball(layer->getChildByTag(layer->balls_total));

    layer->createBallForPlayer(layer->player2->getSide());
    layer->player2->add_ball(layer->getChildByTag(layer->balls_total));

    cocos2d::log(("name is " + name).c_str());
    cocos2d::log(("ip is " + ip).c_str());
    char* secondname = (char *) "Player2";
    layer->_setName(name);
    if (!_GameMode) {
        layer->setIP(ip);
        if (!layer->client.createConnection(const_cast <char*>((layer->ip).c_str()))) {
            cocos2d::log("соединение с сервером  не установлено\n");
        }
        if (!layer->client.handshake(const_cast <char*>((layer->name).c_str()))) {
          cocos2d::log("руку не пожал\n");
        }
        secondname = layer->client.getOpponentName();
        //cocos2d::log(secondname);
        layer->client.launchServesServer();
    }

    cocos2d::TTFConfig labelNameConfig;
    labelNameConfig.fontFilePath = "RUSBoycott.ttf";
    labelNameConfig.fontSize = 48;
    labelNameConfig.outlineSize = 2;

    if (_GameMode == 2) {
        layer->name = "Ultron";
        secondname = (char *) "Human";
    }
    layer->Name1 = cocos2d::Label::createWithTTF(labelNameConfig, layer->name);
    layer->Name1->setPosition((float) (layer->visibleSize.width * 0.25 * LHS), 0);
    layer->Name1->setRotation(50);
    layer->addChild(layer->Name1, 1);

    layer->Name2 = cocos2d::Label::createWithTTF(labelNameConfig, secondname);
    layer->Name2->setPosition((float) (layer->visibleSize.width * 0.25 * RHS), 0);
    layer->Name2->setRotation(-65);
    layer->addChild(layer->Name2);

    return scene;
}

bool GameScene::init() {
    if ( !cocos2d::Layer::init() )
    {
        return false;
    }

    delay = 0;

    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
        auto TouchListener = cocos2d::EventListenerTouchAllAtOnce::create();
        TouchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
        TouchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
        TouchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, this);
    }

    if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
            || CC_TARGET_PLATFORM ==  CC_PLATFORM_MAC) {
        auto KeyListener = cocos2d::EventListenerKeyboard::create();
        cocos2d::Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
        KeyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
        KeyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(KeyListener, this);
    }

    auto ContactListener = cocos2d::EventListenerPhysicsContact::create();
    ContactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegan, this);
     _eventDispatcher->addEventListenerWithSceneGraphPriority(ContactListener, this);

    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    if (!GameMode) {
        player1 = new PlayerMulti;
        player2 = new PlayerMulti;
    } else {
        player1 = new Player;
        player2 = new Player;
    }

    player1->setSide(LHS);
    player2->setSide(RHS);

    //cocos2d::log("Creating balls");  //Debugging

    balls_total = 0;  

    auto mainbackground = cocos2d::Sprite::create("BackGround.png");
    mainbackground->setPosition(0, visibleSize.width / 10);
    this->addChild(mainbackground, 0);

    cocos2d::TTFConfig labelConfig;
    labelConfig.fontFilePath = "DOCKERTHREE.ttf";
    labelConfig.fontSize = 36;

    cocos2d::TTFConfig labelConfig2;
    labelConfig2.fontFilePath = "DOCKERTHREE.ttf";
    labelConfig2.fontSize = 20;

    cocos2d::TTFConfig labelNameConfig;
    labelNameConfig.fontFilePath = "RUSBoycott.ttf";
    labelNameConfig.fontSize = 48;

    Name1 = cocos2d::Label::createWithTTF(labelNameConfig, name);
    Name1->setPosition((float) (visibleSize.width * 0.2 * LHS), 0);
    Name1->setRotation(45);
    this->addChild(Name1);

    Score1 = cocos2d::Label::createWithTTF(labelConfig, to_string(player1->get_score()));
    Score1->setPosition(visibleSize.width / 7 * LHS, (float) (visibleSize.height * 0.6));
    Score1->setName("Score1");
    this->addChild(Score1);

    BallsLost1 = cocos2d::Label::createWithTTF(labelConfig2, "Balls lost: " + to_string(player1->get_lost_balls()));
    BallsLost1->setPosition(visibleSize.width / 5 * LHS, (float) (visibleSize.height * 0.5));
    this->addChild(BallsLost1);


    Score2 = cocos2d::Label::createWithTTF(labelConfig, to_string(player1->get_score()));
    Score2->setPosition(visibleSize.width / 7 * RHS, (float) (visibleSize.height * 0.6));
    Score2->setName("Score2");
    this->addChild(Score2);

    BallsLost2 = cocos2d::Label::createWithTTF(labelConfig2, "Balls lost: " + to_string(player2->get_lost_balls()));
    BallsLost2->setPosition(visibleSize.width / 5 * RHS, (float) (visibleSize.height * 0.5));
    this->addChild(BallsLost2);

    this->scheduleUpdate();

    auto closeItem = cocos2d::MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameScene::Pause, this));

    closeItem->setPosition(cocos2d::Vec2(visibleSize.width / 2 - closeItem->getContentSize().width / 2 ,
                                         (float) (visibleSize.height * 0.65 - closeItem->getContentSize().height / 2)));
    auto menu = cocos2d::Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, AlwaysFront);

    FlipperData.left = false;
    FlipperData.right = false;
    FlipperData.spring = 0;

    TimeTillVictory = 0;
    LosingPlayerSide = 0;

    temp = 0;

    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
        cocos2d::TTFConfig labelHintConfig;
        labelHintConfig.fontFilePath = "DOCKERTHREE.ttf";
        labelHintConfig.fontSize = 25;
        labelHintConfig.outlineSize = 2;
        auto TouchHint = cocos2d::Label::createWithTTF(labelHintConfig, "   Tilt down to\nrelease the ball!");
        TouchHint->setColor(ccc3(255,0,0));
        TouchHint->setPosition(visibleSize.width / 5 * RHS, (float) (visibleSize.height * 0.43));
        TouchHint->setName("Hint");
        this->addChild(TouchHint);

    }

    return true;
}

void GameScene::Pause(cocos2d::Ref* pSender) {
    auto pauseScene=PauseScene::createScene();

    cocos2d::Director::getInstance()->pushScene(pauseScene);
}

cocos2d::Layer* GameScene::GetPlayField(Builder *ConcreteBuilder) {

    ConcreteBuilder->createBoundary(this);

    ConcreteBuilder->createFlippers(this, LHS, Player1);
    ConcreteBuilder->createFlippers(this, RHS, Player1);
    ConcreteBuilder->createFlippers(this, LHS, Player2);
    ConcreteBuilder->createFlippers(this, RHS, Player2);
    ConcreteBuilder->createObsticles(this);

    ConcreteBuilder->createTripleBumper(this, Player1);
    ConcreteBuilder->createTripleBumper(this, Player2);

    ConcreteBuilder->createSlingshot(this, RHS, Player1);
    ConcreteBuilder->createSlingshot(this, LHS, Player1);
    ConcreteBuilder->createSlingshot(this, RHS, Player2);
    ConcreteBuilder->createSlingshot(this, LHS, Player2);

    ConcreteBuilder->createWall(this, Player1);
    ConcreteBuilder->createWall(this, Player2);

    ConcreteBuilder->createPlunger(this, Player2);
    ConcreteBuilder->createPlunger(this, Player1);
}


void GameScene::createBall(cocos2d::Vec2 BallPosition, int Player) {
    auto sprite = cocos2d::Sprite::create("ball.png");
    sprite->setPosition(BallPosition);
    sprite->setName("Ball");
    this->addChild(sprite, 1000);

    if (GameMode || Player == -1) {
        /*auto circle = cocos2d::PhysicsBody::createCircle(BallRadius, cocos2d::PhysicsMaterial(0.005f, 0.2f, 0.5f));
        circle->setVelocityLimit(BallVelocityLimit);
        circle->setAngularVelocityLimit(BallAngVelocityLimit);
        circle->setCategoryBitmask((int)PhysicsCategory::Ball);
        circle->setContactTestBitmask((int)PhysicsCategory::All);
        sprite->setPhysicsBody(circle);*/
        createBallBody(sprite);
    }

    ++balls_total;
    sprite->setTag(balls_total);
}

void GameScene::createBallBody(Sprite *sprite) {
    const int BallVelocityLimit = 950;
    const int BallAngVelocityLimit = 25;
    const int BallRadius = (const int) (visibleSize.width / 80);
    auto circle = cocos2d::PhysicsBody::createCircle(BallRadius, cocos2d::PhysicsMaterial(0.005f, 0.2f, 0.5f));
    circle->setVelocityLimit(BallVelocityLimit);
    circle->setAngularVelocityLimit(BallAngVelocityLimit);
    circle->setCategoryBitmask((int)PhysicsCategory::Ball);
    circle->setContactTestBitmask((int)PhysicsCategory::All);
    sprite->setPhysicsBody(circle);
    sprite->setPhysicsBody(circle);
}

void GameScene::createBallForPlayer(int side) {
    createBall(cocos2d::Vec2(side * (visibleSize.width / 2 -
                                     this->getChildByName("PlungerStatic")->getContentSize().width / 2),
                             (float) (visibleSize.height * 0.4)), side);
}

void GameScene::flipper_triggered(std::string flipperName) {
    auto Torque = 97250;
    auto flipperDefaultVelocity = 25;
    auto side = get_side(flipperName);
    auto flipper = this->getChildByName(flipperName);
    auto flipperBody = flipper->getPhysicsBody();
    auto flipperSize = flipper->getContentSize();
    flipperBody->resetForces();
    flipperBody->applyTorque(-side * Torque * flipperSize.width * flipperSize.height / 2);
    if (GameMode) {
        flipperBody->setAngularVelocity(-side * flipperDefaultVelocity);
    }
    if (!GameMode && (flipperName == "Player1LeftFlipper" || flipperName == "Player1RightFlipper")) {
        flipperBody->setAngularVelocity(-side * flipperDefaultVelocity);
    }
    //auto audio = SimpleAudioEngine::getInstance();
    //audio->playEffect("flippers.mp3", false, 1.0f, 1.0f, 1.0f);

}

void GameScene::flipper_released(std::string flipperName) {
    auto Torque = 13414;
    auto side = get_side(flipperName);
    auto flipper = this->getChildByName(flipperName);
    auto flipperBody = flipper->getPhysicsBody();
    flipperBody->resetForces();
    auto flipperSize = flipper->getContentSize();
    flipperBody->applyTorque(side * Torque * flipperSize.height * flipperSize.width / 2);
}

void GameScene::plunger_triggered(std::string plungerName) {
    auto Force = -366;
    auto velocityLimit = 250;
    auto plunger = this->getChildByName(plungerName);
    auto plungerBody = plunger->getPhysicsBody();
    plungerBody->setVelocityLimit(velocityLimit);
    auto plungerSize = plunger->getContentSize();
    plungerBody->applyForce(cocos2d::Vec2(0, Force * plungerSize.width * plungerSize.height));

    //cocos2d::log("force applied");  /Debugging
}

void GameScene::plunger_released(std::string plungerName) {
    auto plungerVelocityLimit = 15000;
    auto plunger = this->getChildByName(plungerName)->getPhysicsBody();
    plunger->setVelocityLimit(plungerVelocityLimit);
    plunger->setGravityEnable(false);
    plunger->resetForces();
    if (plungerName == "RightPlunger" && this->getChildByName("Hint")) {
        removeChildByName("Hint");
    }
}

bool GameScene::onContactBegan(cocos2d::PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (nodeA->getName() != "Ball")
        std::swap(nodeA, nodeB);

    //auto audio = SimpleAudioEngine::getInstance();
    //audio->playEffect("bumper.mp3", false, 1.0f, 1.0f, 1.0f);

    if (nodeB->getName() == "Bumper") {

        nodeA->getPhysicsBody()->setVelocity(80 * Vec2(nodeA->getPosition().x - nodeB->getPosition().x,
                                                       nodeA->getPosition().y - nodeB->getPosition().y));
        if (nodeA->getPosition().x < 0) {
            player1->add_score(BumperScore);
        } else {
            player2->add_score(BumperScore);
        }
    }
    if (nodeB->getName() == "Kicker") {
        nodeA->getPhysicsBody()->setVelocity(Vec2(20 * nodeB->getRotation(), 450));
        if (nodeA->getPosition().x < 0) {
            player1->add_score(KickerScore);
        } else {
            player2->add_score(KickerScore);
        }
    }
    return true;
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event *event) {
    if (GameMode) {
        switch(keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                flipper_triggered("Player2LeftFlipper");
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                flipper_triggered("Player2RightFlipper");
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                plunger_triggered("RightPlunger");
                //cocos2d::log("plunger triggered");  //Debugging
                break;
        }
    }
    if (GameMode != 2) {
        switch(keyCode) {
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                flipper_triggered("Player1LeftFlipper");
                FlipperData.left = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                flipper_triggered("Player1RightFlipper");
                FlipperData.right = true;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                plunger_triggered("LeftPlunger");
                //cocos2d::log("plunger triggered");  //Debugging
                break;
            /*case cocos2d::EventKeyboard::KeyCode::KEY_W:
                cocos2d::log("error!!!");  //Debugging
                break;*/
        }
    }
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    if (GameMode) {
        switch(keyCode){
            case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                flipper_released("Player2LeftFlipper");
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                flipper_released("Player2RightFlipper");
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                plunger_released("RightPlunger");
                break;
        }
    }
    if (GameMode != 2) {
        switch(keyCode){
            case cocos2d::EventKeyboard::KeyCode::KEY_A:
                flipper_released("Player1LeftFlipper");
                FlipperData.left = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_D:
                flipper_released("Player1RightFlipper");
                FlipperData.right = false;
                break;
            case cocos2d::EventKeyboard::KeyCode::KEY_S:
                plunger_released("LeftPlunger");
        }
    }
}

void GameScene::onTouchesBegan( const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event ) {
    for ( auto &item: touches ) {
        auto touch = item;
        auto touchX = cocos2d::Director::getInstance()->convertToGL( touch->getLocationInView()).x;

        string flippername;
        if (GameMode == 2) {
            flippername = "Player2";
        } else {
            flippername = "Player1";
        }

        if (touchX < visibleSize.width / 2) {
            flippername += "LeftFlipper";
            flipper_triggered(flippername);
        } else {
            flippername += "RightFlipper";
            flipper_triggered(flippername);
        }
    }
    return;
}

void GameScene::onTouchesMoved( const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event ) {
    for ( auto &item: touches ) {
        auto touch = item;
        if (touch->getLocationInView().y > previousTouchY) {
            move_string_down(1);
        }
        previousTouchY = (int) touch->getLocationInView().y;
    }
    return;
}

void GameScene::onTouchesEnded( const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event ) {
    for ( auto &item: touches ) {
        auto touch = item;

        auto done = TouchesLabels[touch->getID()];
        this->removeChild(done, true);

        std::string flippername;
        if (GameMode == 2) {
            flippername = "Player2";
        } else {
            flippername = "Player1";
        }

        auto touchX = cocos2d::Director::getInstance()->convertToGL( touch->getLocationInView()).x;
        if (touchX < visibleSize.width / 2) {
            flippername += "LeftFlipper";
            flipper_released(flippername);
        } else {
            flippername += "RightFlipper";
            flipper_released(flippername);
        }
        plunger_released("RightPlunger");
    }
    return;
}

void GameScene::update_spring(int player) {
    int multiplier = 1;
    const int angle = 45;

    for (int i = 0; i < 8; ++i) {
        PlayField *field = nullptr;
        auto SegmentName = field->PlayField::Name_generator("Segment", player);
        auto SpringLength = this->
                getChildByName(field->PlayField::Name_generator("Plunger", player))->
                getPosition().y - this->getChildByName("PlungerStatic")->getPosition().y;

        int SegmentHeight = SpringLength / 9;
        int alpha = angle / this->getChildByName("PlungerStatic")->getContentSize().height;
        SegmentName += to_string(i + 1);
        auto Segment = this->getChildByName(SegmentName);
        Segment->setPosition(player * this->
                             getChildByName("PlungerStatic")->getPosition().x,
                             -visibleSize.height / 3 + (i + 1) * SegmentHeight);
        multiplier *= -1;
        Segment->setRotation(alpha * multiplier * SegmentHeight);
    }
}

void GameScene::move_string_down(int player) {
    const int AndroidForce = -10000;
    PlayField *field;
    auto plunger = this->getChildByName(field->Name_generator("Plunger", player));
    //auto previousPosition = plunger ->getPosition();
    //previousPosition.y -= 10;
    plunger->getPhysicsBody()->applyForce(cocos2d::Vec2(0, AndroidForce));
    //plunger->setPosition(previousPosition);
}

void GameScene::Ultron() {

    Player* &player_bot = this->player1;
    auto size = visibleSize;

    auto RightFlipper = this->getChildByName("Player1RightFlipper");
    auto LeftFlipper = this->getChildByName("Player1LeftFlipper");

    auto cos1 = abs(cos(abs(0.52)));
    auto tan1 = abs(tan(abs(0.52)));

    auto RightPosition = RightFlipper->getPosition();
    auto Xright = RightPosition.x;
    auto Yright = RightPosition.y;

    auto LeftPosition = LeftFlipper->getPosition();
    auto Xleft = LeftPosition.x;
    auto Yleft = LeftPosition.y;

    for (size_t i = 0; i < player_bot->get_balls_number(); ++i) {
        auto localX = abs(player_bot->getBall(i)->getPositionX()) - abs(Xright);
        auto localY = abs(player_bot->getBall(i)->getPositionY() - Yright);
        auto localoffset = size.width / 32;
        int chance;
        if (localX > localoffset && localX < RightFlipper->getContentSize().width / 2 *
                cos1 + localoffset && localY < localX * tan1) {
            if (localX < RightFlipper->getContentSize().width * 0.45) {
                chance = rand() % 9;
            } else {
                chance = 6;
            }
            if (chance == 6) {
                flipper_triggered("Player1RightFlipper");
                flipper_released("Player1RightFlipper");
            }
        }

        //if (abs(Xleft - player_bot->getBall(i)->getPositionX()) < size.width / 20
                //&& abs(Yleft - 30 - player_bot->getBall(i)->getPositionY()) < size.width / 20) {
        auto localX1 = (-1) * (abs(player_bot->getBall(i)->getPositionX()) - abs(Xleft));
        auto localY1 = abs(player_bot->getBall(i)->getPositionY() - Yleft);
        if (localX1 > localoffset && localX1 < LeftFlipper->getContentSize().width / 2 *
                cos1 + localoffset && localY1 < localX1 * tan1) {
            if (localX1 < LeftFlipper->getContentSize().width * 0.48) {
                chance = rand() % 9;
            } else {
                chance = 6;
            }
            if (chance == 6) {
                flipper_triggered("Player1LeftFlipper");
                flipper_released("Player1LeftFlipper");
            }
        }
    }
    for (size_t i = 0; i < player_bot->get_balls_number(); ++i) {
        auto ballPosition = player_bot->getBall(i)->getPosition();
        auto PlungerWidth = this->getChildByName("PlungerStatic")->getContentSize().width;
        if (ballPosition.x < (-1) * size.width / 2 + PlungerWidth && ballPosition.y < size.height * 0.35 && ballPosition.y > size.height * 0.25 && !delay) {
            plunger_triggered("LeftPlunger");
            ++delay;
        }
        if (ballPosition.x < (-1) * size.width / 2 + PlungerWidth && ballPosition.y < size.height) {
            ++delay;
        }
        if (delay >= 60) {
            plunger_released("LeftPlunger");
            delay = 0;
        }
    }
}

int GameScene::get_side(std::string name) {
    if (strstr(name.c_str(), "Left")) {
        return LHS;
    }
    if (strstr(name.c_str(), "Right")) {
        return RHS;
    }
    return 0;
}

void GameScene::SendBallPositions() {
    SimpleBall myArr[MAX_COUNT_BALLS];
    for (size_t i = 0; i < player1->get_balls_number(); ++i) {
        myArr[i].x = player1->getBall(i)->getPosition().x;
        myArr[i].y = player1->getBall(i)->getPosition().y;
        myArr[i].rotation = player1->getBall(i)->getRotation();
    }
    client.sendArrBalls((int) player1->get_balls_number(), myArr);
    /*if (temp) {
        cocos2d::log("Sending:");
        for (int i = 0; i < player1->get_balls_number(); ++i) {
            cocos2d::log(("Ball number " + to_string(i) + " with X " + to_string(myArr[i].x) + ", Y " + to_string(myArr[i].y)).c_str());
        }
    }*/
}

void GameScene::updateBallPositions() {
    SimpleBall myArr[MAX_COUNT_BALLS];
    int count;
    if (!client.getArrBallsOpponent(myArr, count)) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        auto ball = player2->getBall(i);
        if (ball != NULL) {
            ball->setPosition(myArr[i].x * (-1), myArr[i].y);
            ball->setRotation((-1) * myArr[i].rotation);
        }
    }
    //if (player2->get_balls_number() > client.getCountArrBallsOpponent()) {
    //    player2->set_balls_number(client.getCountArrBallsOpponent());
    //}
    /*if (temp) {
        cocos2d::log("Getting:");
        for (int i = 0; i < count; ++i) {
            cocos2d::log(("Ball number " + to_string(i) + " with X " + to_string(myArr[i].x) + ", Y " + to_string(myArr[i].y)).c_str());
        }
    }*/
}

void GameScene::checkBalls(Player *&player_to_check, Player *&secondPlayer) {
    int Difference = 4000000;
    if (GameMode == 2) {
        Difference = 13000000;
    }
    //cocos2d::log(("time till victory is " + to_string(TimeTillVictory)).c_str());
    if (secondPlayer->get_score() - player_to_check->get_score() < Difference && TimeTillVictory != 0 && player_to_check->getSide() == LosingPlayerSide) {
        TimeTillVictory = 0;
        LosingPlayerSide = 0;
        this->removeChildByName("Lose");
    }
    if (secondPlayer->get_score() - player_to_check->get_score() >= Difference && TimeTillVictory == 0) {
        ++TimeTillVictory;
        LosingPlayerSide = player_to_check->getSide();
        cocos2d::TTFConfig labelConfigTime;
        labelConfigTime.fontFilePath = "DOCKERTHREE.ttf";
        labelConfigTime.fontSize = 25;

        YouWillLose = cocos2d::Label::createWithTTF(labelConfigTime, "You will lose in " + to_string(10 - TimeTillVictory / 120) + " sec");
        YouWillLose->setPosition(visibleSize.width * 0.22 * player_to_check->getSide(), visibleSize.height * 0.43);
        //cocos2d::log(("Label is at " + to_string(visibleSize.width * 0.25 * player_to_check->getSide())).c_str());
        YouWillLose->setName("Lose");
        //cocos2d::log("Countdown started");
        this->addChild(YouWillLose);
    }
    if (TimeTillVictory != 0) {
        ++TimeTillVictory;
        YouWillLose->setString("You will lose in " + to_string(10 - TimeTillVictory / 120) + " sec");
    }
    if (TimeTillVictory >= 1200 && player_to_check->getSide() == LosingPlayerSide) {
        player_to_check->set_lost();
    }
    for (size_t i = 0; i < player_to_check->get_balls_number(); ++i) {
        if (player_to_check->getBall(i)->getPositionY() < -visibleSize.height / 2) {

            /* Debugging *************************************************/
            //cocos2d::log("ball lost");
            /*std::stringstream info;
            info << "Ball number " << balls_total << " is lost at X " << player_to_check->getBall(i)->getPositionX() <<
                    " Y " << player_to_check->getBall(i)->getPositionY() << ", iterarion number is " << to_string(i);
            cocos2d::log(info.str().c_str());
            /* Debugging *************************************************/

            //if (!GameMode && player_to_check->getSide() == -1) {
            //    SendBallPositions();
            //}

            /* Debugging *************************************************/
            /*for (int j = 0; j < 30; ++j) {
                if (player_to_check->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(player_to_check->getBall(j)->getPositionX()) + ", Y " + to_string(player_to_check->getBall(j)->getPositionY())).c_str());
                }
            }
            /* Debugging *************************************************/

            auto temporaryTag = player_to_check->getBall(i)->getTag();
            player_to_check->delete_ball(i);
            if (player_to_check->getSide() == LHS) {
                this->getChildByTag(temporaryTag)->getPhysicsBody()->removeFromWorld();
            }
            //this->removeChildByTag(temporaryTag);
            this->getChildByTag(temporaryTag)->removeFromParentAndCleanup(true);

            player_to_check->ball_lost();

            player_to_check->add_score(BallLostScore);

            if(!player_to_check->get_balls_number()) {
                createBallForPlayer(player_to_check->getSide());
                player_to_check->add_ball(this->getChildByTag(balls_total));
            }

            /* Debugging *************************************************/
            /*cocos2d::log("after deleting ball:");

            for (int j = 0; j < 30; ++j) {
                if (player_to_check->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(player_to_check->getBall(j)->getPositionX()) + ", Y " + to_string(player_to_check->getBall(j)->getPositionY())).c_str());
                }
            }
            /* Debugging *************************************************/

        } else if (player_to_check->getBall(i)->getPositionX() * player_to_check->getSide() < 0) {

            /* Debugging *************************************************/
            //cocos2d::log(("ball " + to_string(i) + " crossed the border, player " + to_string(player_to_check->getSide())).c_str());
            /* Debugging *************************************************/
            //if (!GameMode && player_to_check->getSide() == -1) {
            //    SendBallPositions();
            //}
            temp = 15;

            /* Debugging *************************************************/
            for (int j = 0; j < 30; ++j) {
                if (player_to_check->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    //cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(player_to_check->getBall(j)->getPositionX()) + ", Y " + to_string(player_to_check->getBall(j)->getPositionY())).c_str());
                }
            }
            /*cocos2d::log("second player:");
            for (int j = 0; j < 30; ++j) {
                if (secondPlayer->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(secondPlayer->getBall(j)->getPositionX()) + ", Y " + to_string(secondPlayer->getBall(j)->getPositionY())).c_str());
                }
            }
            /* Debugging *************************************************/

            auto temporaryTag = player_to_check->getBall(i)->getTag();

            auto CrossingBall = this->getChildByTag(temporaryTag);
            if (GameMode || player_to_check->getSide() == LHS) {
                player_to_check->delete_ball(i);
                secondPlayer->add_ball(CrossingBall);
            }

            /* Debugging *************************************************/
            //cocos2d::log(("ball " + to_string(i) + " crossed the border").c_str());
            /* Debugging *************************************************/

            /* Debugging *************************************************/
            /*cocos2d::log(("player 1 got " + to_string(player1->get_balls_number()) + " balls").c_str());
            cocos2d::log(("player 2 got " + to_string(player2->get_balls_number()) + " balls").c_str());
            cocos2d::log("After deleting crossing ball:");
            /* Debugging *************************************************/

            /* Debugging *************************************************/
            /*for (int j = 0; j < 30; ++j) {
                if (player_to_check->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(player_to_check->getBall(j)->getPositionX()) + ", Y " + to_string(player_to_check->getBall(j)->getPositionY())).c_str());
                }
            }
            cocos2d::log("second player:");
            for (int j = 0; j < 30; ++j) {
                if (secondPlayer->getBall(j) == NULL) {
                    //cocos2d::log(("ball number " + to_string(j) + " is zero").c_str());
                } else {
                    cocos2d::log(("ball number " + to_string(j) + " is at X " + to_string(secondPlayer->getBall(j)->getPositionX()) + ", Y " + to_string(secondPlayer->getBall(j)->getPositionY())).c_str());
                }
            }
            /* Debugging *************************************************/

            if (!GameMode) {
                if (player_to_check->getSide() == LHS) {
                    PhysicsBall *ball = new PhysicsBall;
                    ball->x = CrossingBall->getPosition().x;
                    ball->y = CrossingBall->getPosition().y;
                    ball->rotation = CrossingBall->getRotation();
                    ball->speedX = CrossingBall->getPhysicsBody()->getVelocity().x;
                    ball->speedY = CrossingBall->getPhysicsBody()->getVelocity().y;
                    CrossingBall->getPhysicsBody()->removeFromWorld();
                    client.sendNewBall(*ball);

                    /* Debugging *************************************************/
                    // std::stringstream info;
                    //std::stringstream moreinfo;
                    //info << "Sending ball with X " << ball->x << " Y " << ball->y;
                    //cocos2d::log(info.str().c_str());
                    //moreinfo << "Original ball has X " << CrossingBall->getPositionX() << " Y " << CrossingBall->getPositionY();
                    //cocos2d::log(moreinfo.str().c_str());
                    //cocos2d::log("Sending");
                    /* Debugging *************************************************/

                } else {
                    //cocos2d::log("checking second player");  //Debugging
                    //while ( !client.isPresenceNewBall() ) {}
                    PhysicsBall ball;
                    if (client.getNewBall(ball)) {
                        player_to_check->delete_ball(i);
                        secondPlayer->add_ball(CrossingBall);
                        //cocos2d::log("Newball ready");  //Debugging
                        createBallBody((Sprite*)CrossingBall);
                        CrossingBall->getPhysicsBody()->setVelocity(Vec2(ball.speedX * (-1), ball.speedY));
                        CrossingBall->setRotation(ball.rotation * (-1));
                        //CrossingBall->setPosition(ball.x * (-1), ball.y);

                        /* Debugging *************************************************/
                        /*std::stringstream info;
                        info << "got ball with X " << ball.x << " Y " << ball.y << "\n"
                             << " ball is now at X " << CrossingBall->getPositionX() << " Y " << CrossingBall->getPositionY();

                        cocos2d::log(info.str().c_str());
                        cocos2d::log("creating physics body");
                        /* Debugging *************************************************/
                    }
                }
            }
            player_to_check->add_score(CrossedScore);
            if (!player_to_check->get_balls_number()) {
                createBallForPlayer(player_to_check->getSide());
                player_to_check->add_ball(this->getChildByTag(balls_total));
            }
        }
    }
    if (player_to_check->lost()) {
        //cocos2d::log(to_string(player_to_check->lost()).c_str()); //Debugging
        auto scene = GameOverScene::createScene(player_to_check->getSide(),
                                                secondPlayer->get_score(), GameMode);
        //Director::getInstance()->replaceScene(TransitionFlipX::create(1, scene));
        cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.5, scene, Color3B(0,255,255)));
        if (!GameMode) {
            client.setFinish();
        }
    }
}

void GameScene::update(float) {
    if (temp) {
        --temp;
    }
    Score1->setString(to_string(player1->get_score()));
    BallsLost1->setString("Balls lost: " + to_string(player1->get_lost_balls()));
    BallsLost2->setString("Balls lost: " + to_string(player2->get_lost_balls()));
    if (GameMode) {
        Score2->setString(to_string(player2->get_score()));
    }
    if(GameMode == 2) {
        Ultron();
    }
    for (int i = 0; i < 3; ++i) {
        this->getScene()->getPhysicsWorld()->step(1 / 180.0f);
    }
    if (!GameMode){
        SendBallPositions();
    }
    //cocos2d::log("checking first player");  //Debugging
    if (!GameMode){
        updateBallPositions();
    }
    checkBalls(player1, player2);
    checkBalls(player2, player1);
    //cocos2d::log("checking second player");  //Debugging

    /*Incresed frequency of Physics engine updating will allow to increse
    maximum speed of objects without them flying through each other
    or adding unnessesery bulk to physicsbodies.*/

    update_spring(Player1);
    //cocos2d::log("String updated for Player1");  //Debugging
    update_spring(Player2);
    //cocos2d::log("String updated for Player2");  //Debugging

    if(!GameMode) {
        FlipperData.spring = this->getChildByName("LeftPlunger")->getPositionY();
        FlipperData.score = player1->get_score();
        client.sendFlipperTriggered(FlipperData);
        FlipperTriggered RightData;
        if (client.getFlipperTriggered(RightData)) {
            Score2->setString(to_string(RightData.score));
            player2->set_score(RightData.score);
            this->getChildByName("RightPlunger")->setPositionY(RightData.spring);
            if (RightData.left) {
                flipper_triggered("Player2RightFlipper");
            }
            if (RightData.right) {
                flipper_triggered("Player2LeftFlipper");
            }
            if (!RightData.left) {
                flipper_released("Player2RightFlipper");
            }
            if (!RightData.right) {
                flipper_released("Player2LeftFlipper");
            }
        }
    }
}
