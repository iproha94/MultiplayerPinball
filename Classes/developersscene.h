#ifndef DEVELOPERSSCENE_H
#define DEVELOPERSSCENE_H

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "basescene.h"

class DevelopersScene : public BaseScene
{
public:

    static cocos2d::Scene* createScene();

    static void action(Ref* sender, ui::Widget::TouchEventType type);

    virtual bool init();

    MenuItemLabel* addMenuItemLabel(std::string Text);


    CREATE_FUNC(DevelopersScene)

};

#endif // DEVELOPERSSCENE_H
