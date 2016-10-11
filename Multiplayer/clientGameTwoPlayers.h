#ifndef CLIENT_GAME_TWO_PLAYERS_H
#define	CLIENT_GAME_TWO_PLAYERS_H

#include "baseCSPinball.h"
#include "msgIOPinball.h"
#include  <queue>

using namespace std;

enum class STATE_CLIENT {
  CREATE,
  CONNECT,
  ACTIVE,
  FINISH,
  END
};

struct ArrayBalls {
  int countSimpleBalls;
  SimpleBall arrBallsOpponent[MAX_COUNT_BALLS];
};

class ClientGameTwoPlayers {
public:
  ClientGameTwoPlayers();
  ~ClientGameTwoPlayers();
  bool createConnection(char *ipServer);
  bool createConnection(char *ipServer, int portServer);
  bool handshake(char name[MAX_LENGTH_NAME_PLAYER]);
  bool launchServesServer();
  STATE_CLIENT getState();
  void setFinish();
  char *getMyName();
  char *getOpponentName();

  bool getNewBall(PhysicsBall &ball);
  bool getArrBallsOpponent(SimpleBall arr[MAX_COUNT_BALLS], int &count);
  bool getFlipperTriggered(FlipperTriggered &flipper);

  void sendArrBalls(int count, SimpleBall arr[MAX_COUNT_BALLS]);
  void sendNewBall(PhysicsBall &newBall);
  void sendFlipperTriggered(FlipperTriggered &flipperTriggered);
  
  void clearArrBalls(int countOpp);

protected:
  bool initConnection(char *ipServer, int portServer);
  static void* staticToListenServer(void* arg);
  void toListenServer();
  virtual void toListenServerData(TYPE_OF_MSG code) = 0;
  void setState(STATE_CLIENT state);
  void setMyName(char name[MAX_LENGTH_NAME_PLAYER]);
  void setOpponentName(char name[MAX_LENGTH_NAME_PLAYER]);

  void mutexArrBallsLock();
  void mutexArrBallsUnlock();
  void mutexNewBallLock();
  void mutexNewBallUnlock();
  void mutexFlipperLock();
  void mutexFlipperUnlock();
  

protected:
  int sock;
  struct sockaddr_in addr;
  STATE_CLIENT state;
  char opponentName[MAX_LENGTH_NAME_PLAYER];
  char myName[MAX_LENGTH_NAME_PLAYER];

  queue<ArrayBalls *> queueArrBallsOpponent;
  queue<PhysicsBall *> queueNewBall;
  queue<FlipperTriggered *> queueFlipperTriggered;

  pthread_mutex_t mutexArrBalls;
  pthread_mutex_t mutexNewBall;
  pthread_mutex_t mutexFlipper;
};
#endif	/* CLIENT_GAME_TWO_PLAYERS_H */

