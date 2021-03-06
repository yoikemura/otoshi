//
//  Slot.cpp
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#include "Slot.h"
#include "Config.h"
#include "SimpleAudioEngine.h"

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
    slot_size = this->getContentSize();
    
    return true;
}


void Slot::show() {
    this->setVisible(true);
}

void Slot::hide() {
    this->setVisible(false);
}

void Slot::rotate(CallFunc *cb)
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
    this->lastEventId = decidedSlot.id;

    animation->addSpriteFrameWithFile(decidedSlot.fileName);
    animation->setDelayPerUnit(0.2f);
    animation->setLoops(5);

    Animate* rotate = Animate::create(animation);
    auto seq = Sequence::create(rotate, cb, NULL);
    this->runAction(seq);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("itemslot.mp3");
}

int Slot::getLastEventId()
{
    return this->lastEventId;
}
