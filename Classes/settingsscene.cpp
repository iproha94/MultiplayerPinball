#include "settingsscene.h"
#include <string.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "audioslider.h"



USING_NS_CC;


Scene* SettingsScene::createScene()
{

    auto scene = Scene::createWithPhysics();
    auto layer = SettingsScene::create();
    scene->addChild(layer);

    return scene;

}



bool SettingsScene::init()
{

    if ( !BaseScene::init() )
    {
        return false;
    }

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    float MusicVolume = audio->getBackgroundMusicVolume();
    float EffectsVolume = audio->getEffectsVolume();

    auto mySlider = AudioSlider::create();
    mySlider->setText("Громкость музыки");
    mySlider->setValue(0.0,1.0,MusicVolume);
    mySlider->setPosition(Vec2(current_width,current_height));
    mySlider->setAction(this, cccontrol_selector(SettingsScene::sliderCallBackMusic), extension::Control::EventType::VALUE_CHANGED);


    auto mySlider1 = AudioSlider::create();
    mySlider1->setText("Громкость звуковых эффектов");
    mySlider1->setValue(0.0,1.0,EffectsVolume);
    mySlider1->setPosition(Vec2(current_width,current_height-200));
    mySlider1->setAction(this, cccontrol_selector(SettingsScene::sliderCallBackEffects), extension::Control::EventType::VALUE_CHANGED);

    this->addChild(mySlider1);
    this->addChild(mySlider);

    auto buttonExit = TechnoButton::createButton("technopark/parallelogram/parallelogram_white.png", "technopark/parallelogram/parallelogram_black.png", "technopark/parallelogram/parallelogram_black.png","Назад");
    buttonExit->addTouchEventListener(SettingsScene::action);
    buttonExit->setName("exit");
    buttonExit->setPosition(Vec2(current_width,current_height-400));
    buttonExit->setScale(0.8f);
    this->addChild(buttonExit);


    return true;

}



void  SettingsScene::sliderCallBackMusic(Ref * sender,extension::Control::EventType controlEvent)
{
    auto slider=dynamic_cast<extension::ControlSlider*>(sender);

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setBackgroundMusicVolume(slider->getValue());

    auto slidervalue=slider->getValue();

    log("%f",slidervalue);

}


void  SettingsScene::sliderCallBackEffects(Ref * sender,extension::Control::EventType controlEvent)
{
    auto slider=dynamic_cast<extension::ControlSlider*>(sender);

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->setEffectsVolume(slider->getValue());

    auto slidervalue=slider->getValue();

    log("%f",slidervalue);

}



void SettingsScene::action(Ref* sender, ui::Widget::TouchEventType type){

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

                          Director::getInstance()->popScene();
                          title->setColor(Color3B::BLACK);

                          break;
                    }

                 }

}





