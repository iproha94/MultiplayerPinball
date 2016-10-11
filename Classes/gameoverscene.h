#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "settingsscene.h"
#include "basescene.h"
#include "technobutton.h"
#include "mainmenu.h"

class  GameOverScene : public BaseScene
{
private:

     GameOverScene(){}
     int GameMode;

public:

    static cocos2d::Scene* createScene(int loserSide, int score, int gamemode);

    static void action(Ref* sender, ui::Widget::TouchEventType type);

    virtual bool init();

    void setGameMode(int _gamemode) { GameMode = _gamemode; }
    int getgamemode() { return GameMode; }

    CREATE_FUNC(GameOverScene);


};

#endif // GAMEOVERSCENE_H
