//
//  Slot.cpp
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#include "Slot.h"
#include "Config.h"

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
    
    Sprite::initWithFile("slot_increment.png");
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

int Slot::rotate()
{
    this->isRotating = true;

    Animation* animation;
    animation = Animation::create();
    
    int len = sizeof(SLOT_DATA) / sizeof(SLOT_DATA[0]);

    for (int i = 0; i < len; i++) {
      animation->addSpriteFrameWithFile(SLOT_DATA[i].fileName);
    }

    int randNum = arc4random() % len;
    SLOT decidedSlot = SLOT_DATA[randNum];
    animation->addSpriteFrameWithFile(decidedSlot.fileName);
    
    animation->setDelayPerUnit(0.1f);
    animation->setLoops(5);

    auto callbackRotate = CallFunc::create([this](){
      this->isRotating = false;
    });
    
    Animate* rotate = Animate::create(animation);
    auto seq = Sequence::create(rotate, callbackRotate, NULL);
    this->runAction(seq);
    
    return randNum;
}
