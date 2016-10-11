#pragma once

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "basescene.h"

class PauseScene : public BaseScene
{
private:

    PauseScene(){}

public:

    static cocos2d::Scene* createScene();

    static void action(Ref* sender, ui::Widget::TouchEventType type);

    inline TechnoButton* addButton(std::string normalImage,std::string selectedImage,std::string disableImage,std::string ButtonText,int & current_height,int & current_width);

    virtual bool init();

    CREATE_FUNC(PauseScene);

};


