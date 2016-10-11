#pragma once

#include "Player.h"
#include "clientPinball.h"

class PlayerMulti : public Player {
public:
    void SendBallPositions(ClientPinball *_client, int _justcrossed) {
        static SimpleBall myArr[10];
        for (int i = 0; i < balls_number; ++i) {
            myArr[i].x = Balls[i]->getPosition().x;
            myArr[i].y = Balls[i]->getPosition().y;
            myArr[i].rotation = Balls[i]->getRotation();
        }
        _client->sendArrBalls(balls_number, myArr);
    }
    void updateBallPositions(ClientPinball *_client, int _justcrossed) {
        SimpleBall myArr[MAX_COUNT_BALLS];
        int count;
        _client->getArrBallsOpponent(myArr, count);
        for (int i = 0; i < balls_number; ++i) {
            auto ball = Balls[i];
            ball->setPosition(myArr[i].x * (-1), myArr[i].y);
            ball->setRotation(myArr[i].rotation);
        }
        _client->sendArrBalls(balls_number, myArr);
    }
};
