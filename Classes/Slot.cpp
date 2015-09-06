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
    log("randNum is %i", abs(randNum));
    
    Animation* animation;
    animation = Animation::create();
    
    animation->addSpriteFrameWithFile("slot_fuck.png");
    animation->addSpriteFrameWithFile("slot_jp.png");
    animation->addSpriteFrameWithFile("slot_fever.png");
    animation->addSpriteFrameWithFile("slot_jap.png");
    
    if(abs(randNum) < 2){
        animation->addSpriteFrameWithFile("slot_fuck.png");
    }else if(abs(randNum) < 5){
        animation->addSpriteFrameWithFile("slot_jp.png");
    }else if(abs(randNum) < 7){
        animation->addSpriteFrameWithFile("slot_fever.png");
    }else{
        animation->addSpriteFrameWithFile("slot_jap.png");
    }
    
    animation->setDelayPerUnit(0.1f);
    animation->setLoops(5);
    
    Animate* animate = Animate::create(animation);
    this->runAction(animate);
    
    return randNum;
}


void Slot::update(float dt) {
    
}
