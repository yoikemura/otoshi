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
    Sprite::initWithFile("goma.png");
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

void Slot::update(float dt) {
}
