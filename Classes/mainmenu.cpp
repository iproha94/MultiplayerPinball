#include "mainmenu.h"
#include "pausescene.h"
#include "AppDelegate.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "gameoverscene.h"
#include "developersscene.h"
#include <string.h>
#include "GameScene.h"
#include "multiplayerinputerscene.h"


USING_NS_CC;


Scene* MainMenu::createScene()
{

    auto scene = Scene::createWithPhysics();
    auto layer = MainMenu::create();
    scene->addChild(layer);

    return scene;


}


bool MainMenu::init()
{

    if ( !BaseScene::init() )
    {
        return false;
    }



    //technopark/parallelogram/
    //technopark/rectangle/
    //technopark/rectangle flared-up/
    //technopark/rhombus/
    //technopark/rectangle/white narrow

    auto buttonLogo= TechnoButton::createButton("technopark/rhombus/rhombus_white.png","technopark/rhombus/rhombus_black.png","technopark/rhombus/rhombus_black.png","technopark");
    buttonLogo->addTouchEventListener(MainMenu::action);
    buttonLogo->setName("buttonLogo");
    buttonLogo->setPosition(Vec2(current_width,current_height));
    this->addChild(buttonLogo);

    auto buttonNewBot= TechnoButton::createButton("technopark/parallelogram/parallelogram_white.png","technopark/parallelogram/parallelogram_black.png","technopark/parallelogram/parallelogram_black.png","Против бота");
    buttonNewBot->addTouchEventListener(MainMenu::action);
    buttonNewBot->setName("buttonNewBot");
    buttonNewBot->setPosition(Vec2(current_width-300,current_height-150));
    buttonNewBot->setScale(0.7f);
    this->addChild(buttonNewBot);

    auto buttonNewHotseat= TechnoButton::createButton("technopark/parallelogram/parallelogram_white.png","technopark/parallelogram/parallelogram_black.png","technopark/parallelogram/parallelogram_black.png","Hotseat");
    buttonNewHotseat->addTouchEventListener(MainMenu::action);
    buttonNewHotseat->setName("buttonNewHotseat");
    buttonNewHotseat->setPosition(Vec2(current_width,current_height-150));
    buttonNewHotseat->setScale(0.7f);
    this->addChild(buttonNewHotseat);

    auto buttonNewMultiplayer= TechnoButton::createButton("technopark/parallelogram/parallelogram_white.png","technopark/parallelogram/parallelogram_black.png","technopark/parallelogram/parallelogram_black.png","Мультиплеер");
    buttonNewMultiplayer->addTouchEventListener(MainMenu::action);
    buttonNewMultiplayer->setName("buttonNewMultiplayer");
    buttonNewMultiplayer->setPosition(Vec2(current_width+300,current_height-150));
    buttonNewMultiplayer->setScale(0.7f);
    this->addChild(buttonNewMultiplayer);

    auto buttonSettings = TechnoButton::createButton("technopark/rectangle/rectangle_white_narrow.png","technopark/rectangle/rectangle_black_narrow.png","technopark/rectangle/rectangle_black_narrow.png","Настройки");
    buttonSettings->addTouchEventListener(MainMenu::action);
    buttonSettings->setName("settings");
    buttonSettings->setPosition(Vec2(current_width,current_height-300));
    this->addChild(buttonSettings);

    auto buttonExit = TechnoButton::createButton("technopark/rectangle_flared-up/rectangle_flared-up_white.png","technopark/rectangle_flared-up/rectangle_flared-up_black.png","technopark/rectangle_flared-up/rectangle_flared-up_black.png","Выход");
    buttonExit->addTouchEventListener(MainMenu::action);
    buttonExit->setName("exit");
    buttonExit->setPosition(Vec2(current_width,current_height-450));
    this->addChild(buttonExit);

    return true;

}




void MainMenu::action(Ref* sender, ui::Widget::TouchEventType type){

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
                          exit(0);
                     }

                     if(button->getName()=="buttonNewHotseat"){
                         if (!(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)) {
                            Director::getInstance()->replaceScene(GameScene::createScene(1));
                         }
                     }

                     if(button->getName()=="buttonNewMultiplayer"){
                        Director::getInstance()->replaceScene(MultiplayerInputerScene::createScene());
                     }

                     if(button->getName()=="buttonNewBot"){
                          Director::getInstance()->replaceScene(GameScene::createScene(2));
                     }

                     if(button->getName()=="settings"){
                          Director::getInstance()->pushScene(SettingsScene::createScene());
                     }

                     if(button->getName()=="buttonLogo"){
                          Director::getInstance()->pushScene(DevelopersScene::createScene());
                     }

                     title->setColor(Color3B::BLACK);

                          break;
                    }

                 }

}
