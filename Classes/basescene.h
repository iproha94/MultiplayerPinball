#ifndef BASESCENE_H
#define BASESCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"



class BaseScene : public cocos2d::Layer
{
public:

    static void action(Ref* sender, cocos2d::ui::Widget::TouchEventType type){};

    int current_height;

    int current_width;

    virtual bool init();


    CREATE_FUNC(BaseScene);

};

#endif // BASESCENE_H
