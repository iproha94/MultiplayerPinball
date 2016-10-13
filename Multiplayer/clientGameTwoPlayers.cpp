
#include "clientGameTwoPlayers.h"
#include <pthread.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include  <queue>
#include "cocos2d.h"

using namespace std;

ClientGameTwoPlayers::ClientGameTwoPlayers() {
  this->setState(STATE_CLIENT::CREATE);

  char opponentName[MAX_LENGTH_NAME_PLAYER] = "player 1";
  this->setOpponentName(opponentName);

  char myName[MAX_LENGTH_NAME_PLAYER] = "player 2";
  this->setMyName(myName);
}

ClientGameTwoPlayers::~ClientGameTwoPlayers() {
  STATE_CLIENT code = STATE_CLIENT::END;
  this->setState(code);
  close(sock);
}

bool ClientGameTwoPlayers::createConnection(char *ipServer) {
  return initConnection(ipServer, STD_PORT);
}

bool ClientGameTwoPlayers::createConnection(char *ipServer, int portServer) {
  return initConnection(ipServer, portServer);
}

bool ClientGameTwoPlayers::initConnection(char *ipServer, int portServer) {
  mutexArrBalls = PTHREAD_MUTEX_INITIALIZER;
  mutexNewBall = PTHREAD_MUTEX_INITIALIZER;
  mutexFlipper = PTHREAD_MUTEX_INITIALIZER;

  if (this->getState() != STATE_CLIENT::CREATE) {
    return false;
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return false;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(portServer);
  addr.sin_addr.s_addr = inet_addr(ipServer);

  if (connect(sock, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
    perror("connect");
    return false;
  }

  this->setState(STATE_CLIENT::CONNECT);
  return true;
}

void ClientGameTwoPlayers::clearArrBalls(int countOpp) {
   while (!queueArrBallsOpponent.empty())
    queueArrBallsOpponent.pop();
}

bool ClientGameTwoPlayers::handshake(char name[MAX_LENGTH_NAME_PLAYER]) {
  if (this->getState() != STATE_CLIENT::CONNECT and this->getState() != STATE_CLIENT::END) {
    return false;
  }

  while (!queueArrBallsOpponent.empty())
    queueArrBallsOpponent.pop();

  while (!queueNewBall.empty())
    queueNewBall.pop();

  while (!queueFlipperTriggered.empty())
    queueFlipperTriggered.pop();

  TYPE_OF_MSG code;
  do
    code = receiveTypeMessage(sock); 
  while (code != TYPE_OF_MSG::START);

  sendTypeMessage(sock, TYPE_OF_MSG::START);

  code = receiveTypeMessage(sock);
  if (code != TYPE_OF_MSG::RECEIVE_NAME)
    return false;

  this->setMyName(name);
  send(sock, myName, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);

  code = receiveTypeMessage(sock);
  char oppName[MAX_LENGTH_NAME_PLAYER];
  if (code != TYPE_OF_MSG::SEND_NAME)
    return false;

  recv(sock, oppName, MAX_LENGTH_NAME_PLAYER * sizeof (char), 0);

  this->setOpponentName(oppName);

  this->setState(STATE_CLIENT::ACTIVE);
  return true;
}

bool ClientGameTwoPlayers::launchServesServer() {
  if (this->getState() != STATE_CLIENT::ACTIVE) {
    return false;
  }
  pthread_t threadListen;
  pthread_create(&threadListen, NULL, this->staticToListenServer, (void*) this);
  return true;
}

void* ClientGameTwoPlayers::staticToListenServer(void* arg) {
  ClientGameTwoPlayers* This = (ClientGameTwoPlayers*) arg;
  This->toListenServer();
  return NULL;
}

void ClientGameTwoPlayers::toListenServer() {
  TYPE_OF_MSG code;
  do {
    code = receiveTypeMessage(sock);
    this->toListenServerData(code);
  } while (code != TYPE_OF_MSG::FINISH and this->getState() == STATE_CLIENT::ACTIVE);

  if (this->getState() != STATE_CLIENT::ACTIVE) {
    sendTypeMessage(sock, TYPE_OF_MSG::FINISH);
    code = receiveTypeMessage(sock);
  }

  if (code == TYPE_OF_MSG::FINISH) {
    this->setState(STATE_CLIENT::FINISH);
  }

  this->setState(STATE_CLIENT::END);
}

STATE_CLIENT ClientGameTwoPlayers::getState() {
  return this->state;
}

void ClientGameTwoPlayers::setState(STATE_CLIENT state) {
  this->state = state;
}

void ClientGameTwoPlayers::setFinish() {
  setState(STATE_CLIENT::FINISH);
}

char *ClientGameTwoPlayers::getMyName() {
  return myName;
}

char *ClientGameTwoPlayers::getOpponentName() {
  return opponentName;
}

void ClientGameTwoPlayers::setMyName(char name[MAX_LENGTH_NAME_PLAYER]) {
  strcpy(this->myName, name);
}

void ClientGameTwoPlayers::setOpponentName(char name[MAX_LENGTH_NAME_PLAYER]) {
  strcpy(this->opponentName, name);
}

bool ClientGameTwoPlayers::getNewBall(PhysicsBall &ball) {
  mutexNewBallLock();
  if (queueNewBall.empty()) {
    mutexNewBallUnlock();
    return false;
  }

  PhysicsBall *newBall = queueNewBall.front();
  queueNewBall.pop();
  mutexNewBallUnlock();
  ball = *newBall;

  delete newBall;
  return true;
}

bool ClientGameTwoPlayers::getFlipperTriggered(FlipperTriggered &flipper) {
  mutexFlipperLock();
  if (queueFlipperTriggered.empty()) {
    mutexFlipperUnlock();
    return false;
  }

  FlipperTriggered *flipperTriggered = queueFlipperTriggered.front();
  queueFlipperTriggered.pop();
  mutexFlipperUnlock();
  flipper = *flipperTriggered;
  delete flipperTriggered;

  return true;
}

bool ClientGameTwoPlayers::getArrBallsOpponent(SimpleBall arr[MAX_COUNT_BALLS], int &count) {
  mutexArrBallsLock();
  if (queueArrBallsOpponent.empty()) {
    mutexArrBallsUnlock();
    return false;
  }

  ArrayBalls *arrayBalls = queueArrBallsOpponent.front();
  queueArrBallsOpponent.pop();
  mutexArrBallsUnlock();
  count = arrayBalls->countSimpleBalls;
  for (int i = 0; i < count; ++i) {
    arr[i] = arrayBalls->arrBallsOpponent[i];
  }
  delete arrayBalls;

  return true;
}

void ClientGameTwoPlayers::sendArrBalls(int count, SimpleBall arr[MAX_COUNT_BALLS]) {
  sendTypeMessage(sock, TYPE_OF_MSG::ARR_BALL);
  send(sock, &count, sizeof (int), 0);
  send(sock, arr, count * sizeof (SimpleBall), 0);
}

void ClientGameTwoPlayers::sendNewBall(PhysicsBall &newBall) {
  sendTypeMessage(sock, TYPE_OF_MSG::NEW_BALL);
  send(sock, &newBall, sizeof (newBall), 0);
}

void ClientGameTwoPlayers::sendFlipperTriggered(FlipperTriggered &flipperTriggered) {
  sendTypeMessage(sock, TYPE_OF_MSG::FLIPPER_TRIGGERED);
  send(sock, &flipperTriggered, sizeof (flipperTriggered), 0);
}

void ClientGameTwoPlayers::mutexArrBallsLock() {
  pthread_mutex_lock(&mutexArrBalls);
}

void ClientGameTwoPlayers::mutexArrBallsUnlock() {
  pthread_mutex_unlock(&mutexArrBalls);
}

void ClientGameTwoPlayers::mutexNewBallLock() {
  pthread_mutex_lock(&mutexNewBall);
}

void ClientGameTwoPlayers::mutexNewBallUnlock() {
  pthread_mutex_unlock(&mutexNewBall);
}

void ClientGameTwoPlayers::mutexFlipperLock() {
  pthread_mutex_lock(&mutexFlipper);
}

void ClientGameTwoPlayers::mutexFlipperUnlock() {
  pthread_mutex_unlock(&mutexFlipper);
}

