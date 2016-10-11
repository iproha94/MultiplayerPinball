#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene(int loserSide, int score) {
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    cocos2d::log("Game over");
    scene->addChild(layer);
    
    std::string text;
    if (loserSide == -1) {
        text = "Player 2 won!";
    } else {
        text = "Player 1 won!";
    }
    auto label = Label::createWithSystemFont(text, "Arial", 96);
    label->setPosition(500,350);
    scene->addChild(label, 1000);

    return scene;
}

bool GameOverScene::init() {
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}
    
