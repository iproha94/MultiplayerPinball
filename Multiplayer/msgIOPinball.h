#ifndef MSGIOPINBALL_H
#define	MSGIOPINBALL_H

#include "baseCSPinball.h"
#include <pthread.h>
#include <cstring>
#include <limits.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_COUNT_BALLS 30

typedef struct {
  float x;
  float y;
  float rotation;
} SimpleBall;

typedef struct {
  float x;
  float y;
  float speedX;
  float speedY;
  float rotation;
} PhysicsBall;

typedef struct {
  bool left;
  bool right;
  float spring;
  int score;
} FlipperTriggered;

//receive - принимать
//send - отправлять

enum class TYPE_OF_MSG {
  FINISH,
  START,
  RECEIVE_NAME,
  SEND_NAME,
  NEW_BALL,
  ARR_BALL,
  FLIPPER_TRIGGERED
};

static int sendall(int s, void *buf, int len, int flags) {
  int total = 0;
  int n;

  while (total < len) {
    n = send(s, (char *)buf + total, len - total, flags);
    if (n == -1) {
      break;
    }
    total += n;
  }

  return (n == -1 ? -1 : total);
}

static int recvall(int s, void *buf, int len, int flags) {
  int total = 0;
  int n;

  while (total < len) {
    n = recv(s, (char *)buf + total, len - total, flags);
    if (n == -1) {
      break;
    }
    total += n;
  }

  return (n == -1 ? -1 : total);
}

//--------------------IO type of message

static void sendTypeMessage(int sock, TYPE_OF_MSG code) {
  send(sock, &code, sizeof (code), 0);
}

static TYPE_OF_MSG receiveTypeMessage(int sock) {
  TYPE_OF_MSG code;
  recv(sock, &code, sizeof (code), 0);
  return code;
}

//--------------------IO the start game

static void sendStartGame(PairPlayers* pair) {
  TYPE_OF_MSG code = TYPE_OF_MSG::START;
  sendTypeMessage(pair->playerOne, code);
  sendTypeMessage(pair->playerTwo, code);
}

static bool receiveStartGame(PairPlayers* pair) {
  TYPE_OF_MSG code = receiveTypeMessage(pair->playerOne);
  if (code != TYPE_OF_MSG::START)
    return false;

  return receiveTypeMessage(pair->playerTwo) == TYPE_OF_MSG::START;
}

//--------------------IO finish the game

static int sendFinishGame(PairPlayers* pair) {
  sendTypeMessage(pair->playerOne, TYPE_OF_MSG::FINISH);
  sendTypeMessage(pair->playerTwo, TYPE_OF_MSG::FINISH);
}

static bool receiveFinishGame(PairPlayers* pair) {
  if (receiveTypeMessage(pair->playerOne) != TYPE_OF_MSG::FINISH)
    return false;

  return receiveTypeMessage(pair->playerTwo) == TYPE_OF_MSG::FINISH;
}

//--------------------IO name of players

static bool receiveNamePlayers(PairPlayers* pair, char namePlayer1[MAX_LENGTH_NAME_PLAYER], char namePlayer2[MAX_LENGTH_NAME_PLAYER]) {
  sendTypeMessage(pair->playerOne, TYPE_OF_MSG::RECEIVE_NAME);
  recv(pair->playerOne, namePlayer1, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);

  sendTypeMessage(pair->playerTwo, TYPE_OF_MSG::RECEIVE_NAME);
  recv(pair->playerTwo, namePlayer2, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);

  return true;
}

static void sendNamePlayers(PairPlayers* pair, char namePlayer1[MAX_LENGTH_NAME_PLAYER], char namePlayer2[MAX_LENGTH_NAME_PLAYER]) {
  sendTypeMessage(pair->playerOne, TYPE_OF_MSG::SEND_NAME);
  send(pair->playerOne, namePlayer2, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);

  sendTypeMessage(pair->playerTwo, TYPE_OF_MSG::SEND_NAME);
  send(pair->playerTwo, namePlayer1, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);
}

#endif	/* MSGIOPINBALL_H */

