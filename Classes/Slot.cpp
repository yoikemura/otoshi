//
//  Slot.cpp
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#include "Slot.h"


USING_NS_CC;
using namespace cocos2d;

Slot::Slot()
{
}

Slot* Slot::create()
{
    Slot *ret = new Slot();
    if (ret && ret->initWithFile())
    {
        ret->autorelease();
        ret->retain();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool Slot::initWithFile()
{
    
    Sprite::initWithFile("slot_fever.png");
    size = Director::getInstance()->getWinSize();
    this->setScale(0.5f);
    slot_size = this->getContentSize();
    
    return true;
}


void Slot::show() {
    this->setVisible(true);
}

void Slot::hide() {
    this->setVisible(false);
}

int Slot::rotation()
{
    //呼ばれたらRandomな 1 ~ 9を数値として持つ。
    int randNum = 10 - (arc4random() % 21);
    
    Animation* animation;
    animation = Animation::create();
    
    animation->addSpriteFrameWithFileName("slot_fuck.png");
    animation->addSpriteFrameWithFileName("slot_jp.png");
    animation->addSpriteFrameWithFileName("slot_fever.png");
    animation->addSpriteFrameWithFileName("slot_jap.png");
    animation->setDelayPerUnit(0.1f);
    animation->setLoops(3);
    
    Animate* animate = Animate::create(animation);
    this->runAction(animate);
    
    return randNum;
}


void Slot::update(float dt) {
    
}
