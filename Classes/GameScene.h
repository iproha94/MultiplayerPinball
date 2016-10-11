#pragma once

#include "PlayField.h"
#include "cocos2d.h"
#include "Player.h"
#include "PlayerMulti.h"
#include <string>
#include "clientPinball.h"
#include <ctime>

#define AlwaysFront 1000
#define LHS -1
#define RHS 1
#define Player1 -1
#define Player2 1

#define KickerScore 150000
#define BumperScore 200000
#define BallLostScore -800000
#define CrossedScore 300000

class GameScene : public cocos2d::Layer {

public:
    static cocos2d::Scene* createScene(int _GameMode, std::string ip = "0.0.0.0", std::string name = "Player1");

    virtual bool init();
    
    void createBall(cocos2d::Vec2 BallPosition, int Player);
    void createBallForPlayer(int side);
    void createBallBody(cocos2d::Sprite *sprite);

    void SendBallPositions();
    void updateBallPositions();

    void Ultron();

    void setGameMode(int _GameMode) { GameMode = _GameMode; }

    CREATE_FUNC(GameScene)

    void update(float) override;
    
private:
    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void checkBalls(Player *&player_to_check, Player *&secondPlayer);

    bool onContactBegan(cocos2d::PhysicsContact &contact);

    void Pause(cocos2d::Ref* pSender);
    void flipper_triggered(std::string flipperName);
    void flipper_released(std::string flipperName);

    void plunger_triggered(std::string plungerName);
    void plunger_released(std::string plungerName);

    void update_spring(int player);
    void move_string_down(int player);

    int get_side(std::string name);

    cocos2d::Layer *GetPlayField(Builder *ConcreteBuilder);

    void setIP(std::string _ip) { ip = _ip; }
    void _setName(std::string _name) { name = _name; }

private:
    cocos2d::Size visibleSize;
    int balls_total;
    int Offsite;
    int Score;
    void PausePressed(int _score);
    void BallLost(int _score);
    cocos2d::Vec2 Origin;
    ClientPinball client;
    
    const static int MAX_TOUCHES = 5;
    cocos2d::Label* TouchesLabels[MAX_TOUCHES];

    Player* player1;
    Player* player2;

    cocos2d::Label * Score1;
    cocos2d::Label * Score2;

    cocos2d::Label *BallsLost1;
    cocos2d::Label *BallsLost2;

    cocos2d::Label *Name1;
    cocos2d::Label *Name2;

    cocos2d::Label *YouWillLose;

    int delay;
    int previousTouchY;

    int GameMode;

    FlipperTriggered FlipperData;

    std::string name;
    std::string ip;

    int TimeTillVictory;

    int LosingPlayerSide;

    int temp;
};

