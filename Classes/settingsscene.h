#ifndef SETTINGSSCENE_H
#define SETTINGSSCENE_H


#include "cocos2d.h"
#include "basescene.h"
#include "extensions/GUI/CCControlExtension/CCControl.h"
#include "ui/CocosGUI.h"



class SettingsScene : public BaseScene
{
public:

    static cocos2d::Scene* createScene();

    void  sliderCallBackMusic(Ref * sender,cocos2d::extension::Control::EventType controlEvent);

    void  sliderCallBackEffects(Ref * sender,cocos2d::extension::Control::EventType controlEvent);

    static void  action(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    virtual bool init();


    CREATE_FUNC(SettingsScene);

};


#endif // SETTINGSSCENE_H









