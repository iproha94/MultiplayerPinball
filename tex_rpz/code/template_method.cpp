class ClientPinball : public ClientGameTwoPlayers {
    virtual void toListenServerData(TYPE_OF_MSG &code) {
        do {
            if (code == TYPE_OF_MSG::ARR_BALL) {
                recv(sock, &countSimpleBalls, sizeof (countSimpleBalls), 0);
                recv(sock, arrBallsOpponent, countSimpleBalls * sizeof (SimpleBall), 0);
            } else if (code == TYPE_OF_MSG::NEW_BALL) {
                recv(sock, &newBall, sizeof (newBall), 0);
                this->setPresenceNewBall(true);
            }
            
            code = receiveTypeMessage(sock);
        } while (code != TYPE_OF_MSG::FINISH and this->getState() == STATE_CLIENT::ACTIVE);
    }
};
