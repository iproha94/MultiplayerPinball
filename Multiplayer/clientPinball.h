/* 
 * File:   clientPinball.h
 * Author: ilyapc-ubuntu
 *
 * Created on 24 Май 2015 г., 19:38
 */

#ifndef CLIENTPINBALL_H
#define	CLIENTPINBALL_H

#include "clientGameTwoPlayers.h"
#include <stdio.h>

class ClientPinball : public ClientGameTwoPlayers {

  virtual void toListenServerData(TYPE_OF_MSG code) {
      if (code == TYPE_OF_MSG::FLIPPER_TRIGGERED) {
        FlipperTriggered *flipperTriggered = new FlipperTriggered;
        mutexFlipperLock();
        recv(sock, flipperTriggered, sizeof (FlipperTriggered), 0);
        queueFlipperTriggered.push(flipperTriggered);
        mutexFlipperUnlock();
      } else if (code == TYPE_OF_MSG::ARR_BALL) {
        ArrayBalls *arrayBalls = new ArrayBalls;
        mutexArrBallsLock();
        recv(sock, &(arrayBalls->countSimpleBalls), sizeof (arrayBalls->countSimpleBalls), 0);
        recv(sock, arrayBalls->arrBallsOpponent, arrayBalls->countSimpleBalls * sizeof (SimpleBall), 0);
        queueArrBallsOpponent.push(arrayBalls);
        mutexArrBallsUnlock();
      } else if (code == TYPE_OF_MSG::NEW_BALL) {
        PhysicsBall *physicsBall = new PhysicsBall;
        mutexNewBallLock();
        recv(sock, physicsBall, sizeof (PhysicsBall), 0);
        queueNewBall.push(physicsBall);
        mutexNewBallUnlock();
      } 
  }
};

#endif	/* CLIENTPINBALL_H */

