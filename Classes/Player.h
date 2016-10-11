#pragma once

#include "cocos2d.h"
#include "clientPinball.h"

USING_NS_CC;

class Player {
public:
    Player();
    void add_score(int value) { score += value; }
    int get_score() { return score; }
    bool lost();
    void set_lost() { balls_lost = 10; }
    void set_score(int _score) { score = _score; }
    void set_balls_number(int new_balls_number) { balls_number = (size_t) new_balls_number; }
    void ball_lost() { ++balls_lost; }
    int get_lost_balls() { return (int) balls_lost; }

    virtual void SendBallPositions(ClientPinball *_client, int _justcrossed) {}
    virtual void updateBallPositions(ClientPinball *_client, int _justcrossed) {}

    size_t get_balls_number() { return balls_number; }
    Node* getBall(size_t number);

    void add_ball(Node* ball);
    void delete_ball(size_t number);

    void setSide(int value) { side = value; }
    int getSide() { return side; }
protected:
    Node* Balls[30];
    size_t balls_number;
private:
    size_t balls_lost;
    int side;
    int score;
};
