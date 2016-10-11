#ifndef TECHNOBUTTON_H
#define TECHNOBUTTON_H

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "settingsscene.h"

using namespace cocos2d;

class TechnoButton:public cocos2d::ui::Button
{
public:
    TechnoButton();

    static TechnoButton* createButton(std::string  normalImage,std::string  selectedImage,std::string  disableImage,std::string  ButtonText );

    virtual bool init();

    CREATE_FUNC(TechnoButton);


};







#endif // TECHNOBUTTON_H
