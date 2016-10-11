#ifndef MAINMENU_H
#define MAINMENU_H

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "basescene.h"

class MainMenu: public BaseScene
{
private:

    MainMenu(){}

public:

    static cocos2d::Scene* createScene();

    static void action(Ref* sender, ui::Widget::TouchEventType type);

    virtual bool init();

    CREATE_FUNC(MainMenu);

};


#endif // MAINMENU_H
