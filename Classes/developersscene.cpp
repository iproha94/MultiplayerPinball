#include "developersscene.h"
#include "pausescene.h"
#include "AppDelegate.h"
#include "settingsscene.h"
#include "technobutton.h"
#include "gameoverscene.h"
#include <string.h>


USING_NS_CC;


Scene* DevelopersScene::createScene()
{

    auto scene = Scene::createWithPhysics();
    auto layer = DevelopersScene::create();
    scene->addChild(layer);


    return scene;


}


bool DevelopersScene::init()
{

    if ( !BaseScene::init() )
    {
        return false;
    }


    Vector<MenuItem*> MenuItems;

    auto myLabel1 = addMenuItemLabel("Илья Петухов - Илья Петухов");
    MenuItems.pushBack(myLabel1);

    auto myLabel2 = addMenuItemLabel("Дмитрий Цыганов - Дмитрий Цыганов");
    MenuItems.pushBack(myLabel2);

    auto myLabel3 = addMenuItemLabel("Иванов Евгений - Иванов Евгений");
    MenuItems.pushBack(myLabel3);

    auto myLabel4 = addMenuItemLabel("Александр Фролов - Александр Фролов");
    MenuItems.pushBack(myLabel4);

    auto menu = Menu::createWithArray(MenuItems);
    menu->alignItemsVertically();
    this->addChild(menu, 1);


    auto emitter = ParticleFireworks::create();
    emitter->setPosition(Vec2(current_width,current_height-current_height/9));
    addChild(emitter, 10);


    auto buttonExit = TechnoButton::createButton("technopark/rectangle_flared-up/rectangle_flared-up_white.png","technopark/rectangle_flared-up/rectangle_flared-up_black.png","technopark/rectangle_flared-up/rectangle_flared-up_black.png","Выход");
    buttonExit->addTouchEventListener(DevelopersScene::action);
    buttonExit->setName("exit");
    buttonExit->setPosition(Vec2(current_width,current_height-450));
    this->addChild(buttonExit);

    return true;

}



void DevelopersScene::action(Ref* sender, ui::Widget::TouchEventType type){

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
                     }

                     title->setColor(Color3B::BLACK);

                          break;
                    }

                 }

}



MenuItemLabel* DevelopersScene::addMenuItemLabel(std::string Text){

    TTFConfig labelConfig;
    labelConfig.fontFilePath = "fonts/arial.ttf";
    labelConfig.fontSize = 36;
    labelConfig.glyphs = GlyphCollection::DYNAMIC;
    labelConfig.outlineSize = 0;
    labelConfig.customGlyphs = nullptr;
    labelConfig.distanceFieldEnabled = false;

    auto myLabel1 = Label::createWithTTF(labelConfig,Text);
    myLabel1->enableOutline(Color4B::BLACK, 1);

    auto closeItem1 = MenuItemLabel::create(myLabel1);

    return closeItem1;

}



