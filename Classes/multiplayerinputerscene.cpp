#include "multiplayerinputerscene.h"
#include "settingsscene.h"
#include <string.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "audioslider.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"



USING_NS_CC;


Scene* MultiplayerInputerScene::createScene()
{

    auto scene = Scene::createWithPhysics();
    auto layer = MultiplayerInputerScene::create();
    scene->addChild(layer);
    return scene;

}



bool MultiplayerInputerScene::init()
{

    if ( !BaseScene::init() )
    {
        return false;
    }


    ui::TextField* textField = ui::TextField::create("Введите имя","fonts/arial.ttf",30);
    textField->setMaxLengthEnabled(true);
    textField->setMaxLength(20);
    textField->setName("inputName");
    textField->setPosition(Vec2(current_width, current_height));
    textField->addEventListener(CC_CALLBACK_2(MultiplayerInputerScene::textFieldEvent, this));
    textField->setColor(Color3B::BLACK);
    addChild(textField);

    ui::TextField* textFieldIP = ui::TextField::create("Введите IP","fonts/arial.ttf",30);
    textFieldIP->setMaxLengthEnabled(true);
    textFieldIP->setMaxLength(27);
    textFieldIP->setName("inputIP");
    textFieldIP->setPosition(Vec2(current_width,current_height/1.2 ));
    textFieldIP->addEventListener(CC_CALLBACK_2(MultiplayerInputerScene::textFieldEvent, this));
    textFieldIP->setColor(Color3B::BLACK);
    addChild(textFieldIP);

    auto buttonOk = TechnoButton::createButton("technopark/rhombus/rhombus_white.png","technopark/rhombus/rhombus_black.png","technopark/rhombus/rhombus_black.png","Ок");
    buttonOk->addTouchEventListener(CC_CALLBACK_2(MultiplayerInputerScene::action, this));
    buttonOk->setName("Ok");
    buttonOk->setPosition(Vec2(current_width,current_height/1.6));
    this->addChild(buttonOk);


    auto buttonExit = TechnoButton::createButton("technopark/parallelogram/parallelogram_white.png", "technopark/parallelogram/parallelogram_black.png", "technopark/parallelogram/parallelogram_black.png","Назад");
    buttonExit->addTouchEventListener(CC_CALLBACK_2(MultiplayerInputerScene::action, this));
    buttonExit->setName("exit");
    buttonExit->setPosition(Vec2(current_width,current_height-current_height/1.3));
    buttonExit->setScale(0.8f);
    this->addChild(buttonExit);


    return true;

}

std::string MultiplayerInputerScene::get_player_name(){
    return player_name;
}

std::string MultiplayerInputerScene::get_server_IP(){
    return server_IP;
}


void MultiplayerInputerScene::action(Ref* sender, ui::Widget::TouchEventType type){

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
                          Director::getInstance()->popScene();
                          title->setColor(Color3B::BLACK);
                          }

                          if(button->getName()=="Ok"){

                          //тут получаем и куда-нибудь сохраняем данные, пока сцена не удалилась
                          auto childName = getChildByName("inputName");
                          ui::TextField* textFieldName = static_cast<ui::TextField*>(childName);
                          player_name = textFieldName->getString();

                          auto childIP = getChildByName("inputIP");
                          ui::TextField* textFieldIP = static_cast<ui::TextField*>(childIP);
                          server_IP = textFieldIP->getString();

                          //Делаем с server_IP и player_name что хотим

                          title->setColor(Color3B::BLACK);
                          Director::getInstance()->replaceScene(GameScene::createScene(0, server_IP, player_name));
                          }
                          break;

                 }

}

}

void MultiplayerInputerScene::textFieldEvent(Ref *pSender, ui::TextField::EventType type)
{
    ui::TextField* textField = static_cast<ui::TextField*>(pSender);

    Vec2 size = textField->getPosition();

    switch (type)
    {
    case ui::TextField::EventType::ATTACH_WITH_IME:
        {
            ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);
            Size screenSize = CCDirector::getInstance()->getWinSize();
            textField->runAction(CCMoveTo::create(0.225f,Vec2(size.x, size.y  + textField->getContentSize().height / 2.0f)));

        }
            break;

    case ui::TextField::EventType::DETACH_WITH_IME:
        {
            ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);
            Size screenSize = CCDirector::getInstance()->getWinSize();
            textField->runAction(CCMoveTo::create(0.225f, Vec2(size.x, size.y-textField->getContentSize().height / 2.0f)));

        }
            break;

    case ui::TextField::EventType::INSERT_TEXT:
        {
            ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);

        }
            break;

    case ui::TextField::EventType::DELETE_BACKWARD:
        {
            ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);

        }
            break;

        default:
            break;
    }
}
