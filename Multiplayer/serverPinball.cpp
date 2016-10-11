#include "msgIOPinball.h"
#include "serverPinball.h"

#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

ServerPinball* ServerPinball::_instance = NULL;

ServerPinball* ServerPinball::instance() {
  if (_instance == 0) {
    _instance = new ServerPinball;
  }
  return _instance;
}

ServerPinball::ServerPinball() {

  stateServer = false;
  stateListening = false;
  stateCopulation = false;
  numberWaitingPlayers = 0;
  numberPairPlayers = 0;

  logger = new Logger;

  logger->printlog("a server's object is created");
}

ServerPinball::~ServerPinball() {
  this->deleteConnection();
  logger->printlog("a server's object has been deleted");
  delete logger;
}

void ServerPinball::destroy() {
  delete _instance;
  _instance = NULL;
}

void ServerPinball::printStateServer() {
  printf("--------------\n");
  printf("Server's states:\n");
  this->isConnect() ? printf("  launched: on\n") : printf("  launched: off\n");
  this->isListening() ? printf("  listening: on\n") : printf("  listening: off\n");
  this->isCopulation() ? printf("  copulation: on\n") : printf("  copulation: off\n");
  printf("  port: %d\n", this->getPort());
  printf("  quantity of waiting players: %d\n", this->getNumberWaitingPlayers());
  printf("  quantity of pair of players: %d\n", this->getNumberPairPlayers());
  printf("  full quantity of games: %d\n", this->getNumberOfGames());
}

void ServerPinball::launchServesServer() {
  pthread_t threadListen;
  pthread_create(&threadListen, NULL, this->staticToListen, (void*) this);
  pthread_t threadCopulate;
  pthread_create(&threadCopulate, NULL, this->staticToCopulate, (void*) this);
}

void ServerPinball::launch() {
  this->launchServesServer();
  sleep(PAUSE_MENU);
  int pointMenu = 1;
  while (pointMenu) {
    this->printStateServer();

    printf("--------------\n");
    printf("Server's menu:\n");
    printf("  1 - connection: create with standart port;\n");
    printf("  2 - connection: create with port's input;\n");
    printf("  3 - connection: delete;\n");
    printf("  4 - listening: to start;\n");
    printf("  5 - listening: to finish;\n");
    printf("  6 - copulation: to start;\n");
    printf("  7 - copulation: to finish;\n");
    printf("  8 - update states;\n");
    printf("  0 - exit.\n");
    printf(">> ");
    if (scanf("%d", &pointMenu) != 1) {
      continue;
    }
    printf("--------------\n");
    printf("\n\n\n\n");

    switch (pointMenu) {
      case 1:
        this->createConnection();
        break;
      case 2:
        printf("Input port: ");
        int p;
        if (scanf("%d", &p) == 1) {
          this->createConnection(p);
        }
        break;
      case 3:
        this->deleteConnection();
        break;
      case 4:
        this->startListen();
        break;
      case 5:
        this->finishListen();
        break;
      case 6:
        this->startCopulate();
        break;
      case 7:
        this->finishCopulate();
        break;
      case 8:
        break;
    }
  }
}

bool ServerPinball::createConnection() {
  if (!this->isConnect()) {
    stateServer = this->initConnection(STD_PORT);
  } else {
    printf("Server already exists\n");
  }
  return this->isConnect();
}

bool ServerPinball::createConnection(int port) {
  if (!this->isConnect()) {
    stateServer = this->initConnection(port);
  } else {
    printf("Server already exists\n");
  }
  return this->isConnect();
}

bool ServerPinball::initConnection(int port) {
  w_lock = PTHREAD_MUTEX_INITIALIZER;
  p_lock = PTHREAD_MUTEX_INITIALIZER;

  struct sockaddr_in addr;

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0) {
    perror("socket");
    return false;
  }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(listener, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
    perror("bind");
    return false;
  }

  this->setPort(port);
  listen(listener, MAX_COUNT_PAIR_PLAYERS * 2 + MAX_LENGTH_QUEUE_WAITING_PLAYERS);
  logger->printlog("server is created");
  printf("server is created\n");
  return true;
}

void ServerPinball::deleteConnection() {
  if (this->isConnect()) {
    this->finishListen();
    pthread_mutex_lock(&w_lock);
    for (int i = 0; i < this->getNumberWaitingPlayers(); ++i) {
      close(queueWaitingPlayers[i]);
    }
    this->setNumberWaitingPlayers(0);
    pthread_mutex_unlock(&w_lock);    

    this->finishCopulate();
    pthread_mutex_lock(&p_lock);
    for (int i = 0; i < this->getNumberPairPlayers(); ++i) {
      sendFinishGame(pairPlayers[i]);
      close(pairPlayers[i]->playerOne);
      close(pairPlayers[i]->playerTwo);
    }
    this->setNumberPairPlayers(0);
    pthread_mutex_unlock(&p_lock);    

    this->setPort(0);
    close(listener);
    stateServer = false;

    logger->printlog("server is deleted");
    printf("server is deleted\n");
  } else {
    printf("server doesn't exist yet\n");
  }
}

void* ServerPinball::staticToListen(void* arg) {
  ServerPinball* This = (ServerPinball*) arg;
  This->toListen();
  return NULL;
}

void ServerPinball::startListen() {
  if (!this->isConnect()) {
    printf("server doesn't exist yet\n");
    return;
  }

  if (this->isListening()) {
    printf("server is already listening\n");
    return;
  }
  logger->printlog("listening has started");
  stateListening = true;
}

void ServerPinball::finishListen() {
  if (!this->isConnect()) {
    printf("server doesn't exist yet\n");
    return;
  }

  if (!this->isListening()) {
    printf("server doesn't already listen\n");
    return;
  }
  logger->printlog("listening has finished");
  stateListening = false;
}

void ServerPinball::toListen() {
  logger->printlog("ready to start listening");
  printf("...ready to start listening\n");
  while (1) {
    if (this->isConnect() and this->isListening() and this->getNumberWaitingPlayers() < MAX_LENGTH_QUEUE_WAITING_PLAYERS) {
      //<error> при остановке слушания, еще один сокет всё равно добавится
      int sock = accept(listener, NULL, NULL);
      if (sock < 0) {
        perror("accept");
        exit(3);
      }
      
      pthread_mutex_lock(&w_lock);
      logger->printlog("waiting player has joined");
      queueWaitingPlayers[this->getNumberWaitingPlayers()] = sock;
      this->setNumberWaitingPlayers(this->getNumberWaitingPlayers() + 1);
      pthread_mutex_unlock(&w_lock);
    }
  }
}

void* ServerPinball::staticToCopulate(void* arg) {
  ServerPinball* This = (ServerPinball*) arg;
  This->toCopulate();
  return NULL;
}

void ServerPinball::startCopulate() {
  if (!this->isConnect()) {
    printf("server doesn't exist yet\n");
    return;
  }

  if (this->isCopulation()) {
    printf("server is already copylating\n");
    return;
  }
  logger->printlog("copulating has started");
  stateCopulation = true;
}

void ServerPinball::finishCopulate() {
  if (!this->isConnect()) {
    printf("server doesn't exist yet\n");
    return;
  }

  if (!this->isCopulation()) {
    printf("server doesn't already copulate\n");
    return;
  }
  logger->printlog("copulating has finished");
  stateCopulation = false;
}

void ServerPinball::toCopulate() {
  logger->printlog("ready to start copulating");
  printf("...ready to start copulating\n");
  while (1) {
    if (this->isConnect() and this->isCopulation()
            and this->getNumberPairPlayers() < MAX_COUNT_PAIR_PLAYERS
            and this->getNumberWaitingPlayers() > 1) {
      PairPlayers *pair = new PairPlayers();
      this->setNumberOfGames(this->getNumberOfGames() + 1);
      
      pthread_mutex_lock(&w_lock);
      pair->game_id = this->getNumberOfGames();
      pair->playerOne = queueWaitingPlayers[this->getNumberWaitingPlayers() - 1];
      pair->playerTwo = queueWaitingPlayers[this->getNumberWaitingPlayers() - 2];
      pair->state = STATE_OF_GAME::START;
      this->setNumberWaitingPlayers(this->getNumberWaitingPlayers() - 2);
      pthread_mutex_unlock(&w_lock);
      
      pthread_mutex_lock(&p_lock);
      pairPlayers[this->getNumberPairPlayers()] = pair;
      this->setNumberPairPlayers(this->getNumberPairPlayers() + 1);
      logger->printlog("a pair of waiting players is united");
      pthread_mutex_unlock(&p_lock);

      pthread_t threadProcessGame;
      DataInProcessGame *arg = new DataInProcessGame();
      arg->newThis = this;
      arg->pairPlayers = pair;
      arg->p_mutex = &p_lock;
      pthread_create(&threadProcessGame, NULL, this->staticProcessGame, (void*) arg);
    }
  }
}

void* ServerPinball::staticProcessGame(void *arg) {
  PairPlayers *pair = ((DataInProcessGame *) arg)->pairPlayers;
  ServerPinball *This = ((DataInProcessGame *) arg)->newThis;
  pthread_mutex_t *p_lock_arg = ((DataInProcessGame *) arg)->p_mutex;

  char strlog[2 * MAX_LENGTH_NAME_PLAYER + 20];
  sprintf(strlog, "game #%d has started", pair->game_id);
  This->logger->printlog(strlog);

  sendStartGame(pair);
  receiveStartGame(pair);

  char nameP1[MAX_LENGTH_NAME_PLAYER];
  char nameP2[MAX_LENGTH_NAME_PLAYER];

  receiveNamePlayers(pair, nameP1, nameP2);
  sendNamePlayers(pair, nameP1, nameP2);

  sprintf(strlog, "game #%d: %s vs %s", pair->game_id, nameP1, nameP2);
  This->logger->printlog(strlog);

  pthread_t threadProcessPlayer1;
  PairSocks *pair1 = new PairSocks();
  pair1->sock1 = pair->playerOne;
  pair1->sock2 = pair->playerTwo;
  pair1->pair = pair;

  pthread_t threadProcessPlayer2;
  PairSocks *pair2 = new PairSocks();
  pair2->sock1 = pair->playerTwo;
  pair2->sock2 = pair->playerOne;
  pair2->pair = pair;

  pthread_create(&threadProcessPlayer1, NULL, staticToListenPlayer, (void*) pair1);
  pthread_create(&threadProcessPlayer2, NULL, staticToListenPlayer, (void*) pair2);

  while (pair->state != STATE_OF_GAME::FINISH) {
    //sleep(1);
  }

  close(pair->playerOne);
  close(pair->playerTwo);

  sprintf(strlog, "game #%d has finished", pair->game_id);
  This->logger->printlog(strlog);

  pthread_mutex_lock(p_lock_arg);
  This->setNumberPairPlayers(This->getNumberPairPlayers() - 1);
  delete This->pairPlayers[This->getNumberPairPlayers()];
  pthread_mutex_unlock(p_lock_arg);
  return NULL;
}

void* ServerPinball::staticToListenPlayer(void* arg) {
  PairSocks* pair = (PairSocks*) arg;

  PhysicsBall newBall;
  SimpleBall arrBalls[MAX_COUNT_BALLS];
  FlipperTriggered flipperTriggered;
  int countBalls;

  TYPE_OF_MSG code;
  do {
    code = receiveTypeMessage(pair->sock1);

    if (code == TYPE_OF_MSG::FLIPPER_TRIGGERED) {
      recvall(pair->sock1, &flipperTriggered, sizeof (flipperTriggered), 0);
      sendTypeMessage(pair->sock2, code);
      sendall(pair->sock2, &flipperTriggered, sizeof (flipperTriggered), 0);
    } else if (code == TYPE_OF_MSG::ARR_BALL) {
      recvall(pair->sock1, &countBalls, sizeof (countBalls), 0);
      recvall(pair->sock1, arrBalls, countBalls * sizeof (SimpleBall), 0);

      sendTypeMessage(pair->sock2, code);
      sendall(pair->sock2, &countBalls, sizeof (countBalls), 0);
      sendall(pair->sock2, arrBalls, countBalls * sizeof (SimpleBall), 0);
    } else if (code == TYPE_OF_MSG::NEW_BALL) {
      recvall(pair->sock1, &newBall, sizeof (newBall), 0);
      sendTypeMessage(pair->sock2, code);
      sendall(pair->sock2, &newBall, sizeof (newBall), 0);
    }
  } while (code != TYPE_OF_MSG::FINISH and pair->pair->state != STATE_OF_GAME::FINISH);

  if (code == TYPE_OF_MSG::FINISH) {
    sendFinishGame(pair->pair);
    pair->pair->state = STATE_OF_GAME::FINISH;
  }

  return NULL;
}

int ServerPinball::getPort() {
  return port;
}

void ServerPinball::setPort(int port) {
  this->port = port;
}

int ServerPinball::getNumberWaitingPlayers() {
  return numberWaitingPlayers;
}

void ServerPinball::setNumberWaitingPlayers(int value) {
  numberWaitingPlayers = value;
}

int ServerPinball::getNumberPairPlayers() {
  return numberPairPlayers;
}

void ServerPinball::setNumberPairPlayers(int value) {
  numberPairPlayers = value;
}

int ServerPinball::getNumberOfGames() {
  return numberOfGames;
}

void ServerPinball::setNumberOfGames(int value) {
  numberOfGames = value;
}

bool ServerPinball::isConnect() {
  return stateServer;
}

bool ServerPinball::isListening() {
  return stateListening;
}

bool ServerPinball::isCopulation() {
  return stateCopulation;
}
