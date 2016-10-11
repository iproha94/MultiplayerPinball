#ifndef AUDIOSLIDER
#define AUDIOSLIDER


#include <string.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "extensions/GUI/CCControlExtension/CCControlSlider.h"
#include "extensions/GUI/CCControlExtension/CCControl.h"
#include "extensions/GUI/CCControlExtension/CCControlExtensions.h"
#include "AudioEngine.h"
#include "technobutton.h"


class AudioSlider : public Node
{
public:
    enum Direction {
        Vertical,
        Horizontal
    };

    static AudioSlider *create()
    {
        auto ret = new (std::nothrow) AudioSlider();
        if (ret && !ret->init()) {
            delete ret;
            ret = nullptr;
        }
        ret->autorelease();
        return ret;
    }


    float getValue() const
    {
        return _slider->getValue();

    }

    void setValue(float minValue, float maxValue, float value)
    {
        _slider->setMinimumValue(minValue);
        _slider->setMaximumValue(maxValue);
        _slider->setValue(value);

    }

    void setText(std::string str){

        auto label = Label::createWithTTF(str, "fonts/arial.ttf", 25);
        label->setName("label");
        label->setPosition(0,50);

        addChild(label);

    }


    void setAction(Ref* target, cocos2d::extension::Control::Handler action, extension::Control::EventType controlEvents){
         _slider->addTargetWithActionForControlEvents(this, action, extension::Control::EventType::VALUE_CHANGED);
    }

    extension::ControlSlider *_slider;

private:


    AudioSlider():_slider(nullptr)
    {
    }

    bool init()
    {

        _slider = extension::ControlSlider::create("white_rectangle_flared-up.png","black_rectangle_flared-up.png" ,"white_rectangle_narrow.png");
        _slider->setScale(1.0f);
        _slider->setRotation(0);
        addChild(_slider);

        return true;
    }


};



#endif // AUDIOSLIDER

