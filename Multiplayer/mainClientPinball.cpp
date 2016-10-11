#include "clientPinball.h"
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <queue>

using namespace std;

int main() {
  srand(time(NULL));

  char SERVER_IP[16] = "192.168.109.146";
  char myName[MAX_LENGTH_NAME_PLAYER] = "player 11";

  ClientPinball client;

  //printf("Input ip: ");
  //scanf("%s", SERVER_IP);

  if (!client.createConnection(SERVER_IP)) {
    printf("соединение с сервером  не установлено\n");
    return 0;
  } else {
    printf("соединение с сервером  установлено\n");
  }

  //printf("Input name: ");
  //scanf("%s", myName);

  if (!client.handshake(myName)) {
    printf("руку не пожал\n");
    return 0;
  }

  printf("I am %s\nMy opponent is %s\n\n", client.getMyName(), client.getOpponentName());
  client.launchServesServer();


  SimpleBall myArr[MAX_COUNT_BALLS];
  SimpleBall opArr[MAX_COUNT_BALLS];
  PhysicsBall newball;
  FlipperTriggered flipperTriggered;
  int n;
  int count;

  while (client.getState() == STATE_CLIENT::ACTIVE) {
    if (1 == (rand() % 9)) {
      printf("Sending the ball: \n");
      newball.x = rand() % 10;
      newball.y = rand() % 10;
      newball.speedX = (rand() % 90 + 10);
      printf("%lf %lf %lf\n", newball.x, newball.y, newball.speedX);
      client.sendNewBall(newball);
      printf("\n");
    }

    if (1 == (rand() % 5)) {
      printf("Sending the flipperTriggered: \n");
      flipperTriggered.left = rand() % 2;
      flipperTriggered.right = rand() % 2;
      flipperTriggered.spring = (rand() % 90 + 10);
      printf("%d %d %lf\n", flipperTriggered.left, flipperTriggered.right, flipperTriggered.spring);
      client.sendFlipperTriggered(flipperTriggered);
      printf("\n");
    }

    if (1 == (rand() % 7)) {
      printf("Sending the array balls: \n");
      n = rand() % 10;
      for (int i = 0; i < n; ++i) {
        myArr[i].x = rand() % 10;
        myArr[i].y = rand() % 10;
        printf("%lf %lf \n", myArr[i].x, myArr[i].y);
      }
      printf("\n");

      client.sendArrBalls(n, myArr);
    }

    if (client.getNewBall(newball)) {
      printf("\t\tCame a new ball: \n");
      printf("\t\t%lf %lf %lf\n", newball.x, newball.y, newball.speedX);
    }

    if (client.getArrBallsOpponent(opArr, count)) {
      printf("\t\tOpponent's array: \n");
      for (int i = 0; i < count; ++i) {
        printf("\t\t%lf %lf \n", opArr[i].x, opArr[i].y);
      }
      printf("\n");
    }

    if (client.getFlipperTriggered(flipperTriggered)) {
      printf("\t\tOpponent's flipperTriggered:\n");
      printf("\t\t%d %d %lf\n", flipperTriggered.left, flipperTriggered.right, flipperTriggered.spring);
      printf("\n");
    }

    sleep(1);

    if (4 == (rand() % 357)) {
      printf("I am finishing\n");
      client.setFinish();
    }
  }
  
  printf("The end\n");
  scanf("%d", &n);
  return 0;
}
