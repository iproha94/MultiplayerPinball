#include "pausescene.h"
#include "AppDelegate.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "gameoverscene.h"
#include <string.h>


USING_NS_CC;


Scene* PauseScene::createScene()
{

    auto scene = Scene::createWithPhysics();
    auto layer = PauseScene::create();
    scene->addChild(layer);


    return scene;


}


bool PauseScene::init()
{

    if ( !BaseScene::init() )
    {
        return false;
    }


    auto buttonContinue = addButton("technopark/rhombus/rhombus_white.png","technopark/rhombus/rhombus_black.png","technopark/rhombus/rhombus_black.png","Продолжить",current_height,current_width);
    buttonContinue->addTouchEventListener(PauseScene::action);
    buttonContinue->setName("continue");


    auto buttonSettings = addButton("technopark/parallelogram/parallelogram_white.png","technopark/parallelogram/parallelogram_black.png","technopark/parallelogram/parallelogram_black.png","Настройки",current_height,current_width);
    buttonSettings->addTouchEventListener(PauseScene::action);
    buttonSettings->setScale(0.8f);
    buttonSettings->setName("settings");



    auto buttonExit = addButton("technopark/rectangle/rectangle_white_narrow.png","technopark/rectangle/rectangle_black_narrow.png","technopark/rectangle/rectangle_black_narrow.png","Выход",current_height,current_width);
    buttonExit->addTouchEventListener(PauseScene::action);
    buttonExit->setName("exit");


    return true;

}



inline TechnoButton* PauseScene::addButton(std::string normalImage,std::string selectedImage,std::string disableImage,std::string ButtonText,int& current_height,int& current_width){


    auto button = TechnoButton:: createButton(normalImage,selectedImage,disableImage,ButtonText);

    auto size = button->getContentSize();


    button->setPosition(Vec2(current_width,current_height));

    this->addChild(button,100);

    current_height-=(size.height/2+100);


    return button;

}



void PauseScene::action(Ref* sender, ui::Widget::TouchEventType type){

      TechnoButton * button = static_cast<TechnoButton*>(sender);
      Label *title = button->getTitleRenderer();

                 switch (type)
                 {
                    case cocos2d::ui::Widget::TouchEventType::BEGAN:
                    {
                          title->setColor(Color3B::WHITE);
                          break;
                    }

                    case cocos2d::ui::Widget::TouchEventType::MOVED:
                    {
                          title->setColor(Color3B::WHITE);
                          break;
                    }

                    case cocos2d::ui::Widget::TouchEventType::CANCELED:
                    {
                          title->setColor(Color3B::BLACK);
                          break;
                     }

                    case ui::Widget::TouchEventType::ENDED:
                    {

                     if(button->getName()=="exit"){
                          Director::getInstance()->end();
                          #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                          exit(0);
                          #endif
                            }

                     if(button->getName()=="continue"){
                          Director::getInstance()->popScene();
                     }

                     if(button->getName()=="settings"){
                          Director::getInstance()->pushScene(SettingsScene::createScene());
                     }

                     title->setColor(Color3B::BLACK);

                          break;
                    }

                 }

}






