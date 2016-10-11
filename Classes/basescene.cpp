#include "basescene.h"
#include "AppDelegate.h"
#include <string.h>


USING_NS_CC;


bool BaseScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    current_height=visibleSize.height-visibleSize.height/5;
    current_width=visibleSize.width/2;

    //главный фон
    auto background = Sprite::create("background.jpg");
    background->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(background,0);


    return true;

}




