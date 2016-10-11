#ifndef SERVER_PINBALL_H
#define	SERVER_PINBALL_H

#include <pthread.h>
#include "baseCSPinball.h"
#include "logger.h"

#define MAX_COUNT_PAIR_PLAYERS 10
#define MAX_LENGTH_QUEUE_WAITING_PLAYERS 10
#define PAUSE_MENU 1


class ServerPinball;

typedef struct {
  PairPlayers *pairPlayers;
  ServerPinball *newThis;
  pthread_mutex_t *p_mutex;
} DataInProcessGame;

typedef struct {
  int sock1;
  int sock2;
  PairPlayers* pair;
} PairSocks;

class ServerPinball {
public://методы
  static ServerPinball* instance(); //for Singlton
  static void destroy(); //for Singlton
  void printStateServer();
  void launch();
  int getPort();
  void setPort(int port);
  int getNumberWaitingPlayers();
  void setNumberWaitingPlayers(int value);
  int getNumberPairPlayers();
  void setNumberPairPlayers(int value);
  int getNumberOfGames();
  void setNumberOfGames(int value);
  bool isConnect();
  bool isListening();
  bool isCopulation();
  ~ServerPinball();

protected://методы
  ServerPinball();
  
private://методы
  void launchServesServer();
  bool createConnection();
  bool createConnection(int port);
  bool initConnection(int port);
  void deleteConnection();
  void startCopulate();
  void finishCopulate();
  void toCopulate();
  static void* staticToCopulate(void* arg);
  void startListen();
  void finishListen();
  void toListen();
  static void* staticToListen(void* arg);
  static void* staticProcessGame(void *arg);
  static void* staticToListenPlayer(void* arg);
  //virtual void* toListenPlayer(PairSocks* arg) = 0;

private://поля
  int listener;
  int port;
  Logger *logger;
  PairPlayers *pairPlayers[MAX_COUNT_PAIR_PLAYERS];
  int numberPairPlayers;
  int queueWaitingPlayers[MAX_LENGTH_QUEUE_WAITING_PLAYERS];
  int numberWaitingPlayers;
  bool stateServer;
  bool stateListening;
  bool stateCopulation;
  int numberOfGames;
  static ServerPinball* _instance; //for Singlton
  pthread_mutex_t w_lock;
  pthread_mutex_t p_lock;
};

#endif	/* SERVER_PINBALL_H */
