#include "Player.h"

Player::Player() {
    score = 0;
    balls_number = 0;
    balls_lost = 0;
    for (int i = 0; i < 30; ++i) {
        Balls[i] = NULL;
    }
    //side = _side;
}

bool Player::lost() {
    if (balls_lost >= 5) {
        return true;
    }
    return false;
}

void Player::add_ball(Node* ball) {
    Balls[balls_number++] = ball;

    //debagging
}

void Player::delete_ball(size_t number) {
    /*size_t i;
    for(i = number + 1; i < balls_number; ++i) {
        Balls[i - 1] = NULL;
        Balls[i - 1] = Balls[i];
    }
    Balls[balls_number - 1] = NULL;
    --balls_number;*/

    --balls_number;
    Balls[number] = Balls[balls_number];
    Balls[balls_number] = NULL;

    //debagging
}

Node* Player::getBall(size_t number) {
    return Balls[number];
}
