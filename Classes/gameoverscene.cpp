#include "gameoverscene.h"
#include "technobutton.h"
#include "GameScene.h"
#include <string.h>

USING_NS_CC;

Scene* GameOverScene::createScene(int loserSide, int score, int gamemode) {

    auto scene = Scene::create();
    auto layer =  GameOverScene::create();
    scene->addChild(layer);
    cocos2d::log("Game over");

    layer->setGameMode(gamemode);

    std::string text;
    if (loserSide == -1) {
        text = "Player 2 won!";
    } else {
        text = "Player 1 won!";
    }

    auto size = Director::getInstance()->getWinSize();

    TTFConfig labelConfig;
    labelConfig.fontFilePath = "DOCKERTHREE.ttf";
    labelConfig.fontSize = 65;
    labelConfig.outlineSize = 2;

    auto Results = Label::createWithTTF(labelConfig, text);
    Results->setPosition(size.width / 2, size.height * 0.9);
    scene->addChild(Results, 1000);

    auto Score = Label::createWithTTF(labelConfig, text);
    Score->setPosition(size.width / 2, size.height * 0.7);


    std::stringstream ss;
    ss << "Score " << score;
    Score->setString(ss.str().c_str());

    scene->addChild(Score, 1000);

    return scene;
}

bool GameOverScene::init() {

    if ( !BaseScene::init() )
    {
        return false;
    }

    auto buttonExit = TechnoButton::createButton("white.png","black.png","black.png","Выйти в меню");
    buttonExit->addTouchEventListener(GameOverScene::action);
    buttonExit->setName("exit");
    buttonExit->setPosition(Vec2(current_width+200,current_height-250));
    this->addChild(buttonExit);

    auto buttonReplay= TechnoButton::createButton("white.png","black.png","black.png","Реванш");
    buttonReplay->addTouchEventListener(GameOverScene::action);
    buttonReplay->setName("replay");
    buttonReplay->setPosition(Vec2(current_width-200,current_height-250));
    this->addChild(buttonReplay);

    return true;
}


void  GameOverScene::action(Ref* sender, ui::Widget::TouchEventType type){

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
                         for(int i=0;i<3;i++)
                         Director::getInstance()->replaceScene(MainMenu::createScene());
                            }

                     if(button->getName()=="replay"){
                         //играть сново
                         auto scene = GameScene::createScene(1);
                         Director::getInstance()->replaceScene(scene);

                     }


                     title->setColor(Color3B::BLACK);

                          break;
                    }

                 }

}


