class ClientGameTwoPlayers {
public:
    ClientGameTwoPlayers();
    ~ClientGameTwoPlayers();
    STATE_CLIENT getState();
    //остальные методы...

protected:
    virtual void toListenServerData(TYPE_OF_MSG &code) = 0;
    //остальные методы...

protected:
    int sock;
    //остальные поля...
};