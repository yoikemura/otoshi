//
//  Ufo.cpp
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#include "Ufo.h"

USING_NS_CC;
using namespace cocos2d;

Ufo::Ufo()
{
}

Ufo* Ufo::create()
{
    Ufo *ret = new Ufo();
    if (ret && ret->initWithFile())
    {
        ret->autorelease();
        ret->retain();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool Ufo::initWithFile()
{
    Sprite::initWithFile("ufo.png");
    size = Director::getInstance()->getWinSize();
    this->setScale(0.5f);
    
    return true;
}


void Ufo::show() {
    this->setVisible(true);
}

void Ufo::hide() {
    this->setVisible(false);
}

void Ufo::move()
{
}


void Ufo::update(float dt) {
}
