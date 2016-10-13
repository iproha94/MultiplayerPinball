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
