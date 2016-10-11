#include "technobutton.h"
#include "string.h"


USING_NS_CC;
using namespace ui;

 TechnoButton::TechnoButton()
{

}


bool TechnoButton::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Button::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();



    return true;

}

TechnoButton* TechnoButton::createButton(std::string normalImage,std::string  selectedImage,std::string  disableImage,std::string  ButtonText )
{


    TechnoButton * button = static_cast<TechnoButton *>(Button::create(normalImage,selectedImage,disableImage));

    button->setTitleText(ButtonText);
    button->setTitleFontSize(25);



    Label *title = button->getTitleRenderer();
    title->setColor(Color3B::BLACK);

    auto size = button->getContentSize();




    return button;


}

