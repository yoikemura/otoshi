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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    MoveTo* gogo =  MoveTo::create(6.0f, Point(visibleSize.width, this->getBoundingBox().size.height));
    MoveTo* goback = MoveTo::create(6.0f, Point(0, this->getBoundingBox().size.height));
    auto spawn = Spawn::create(gogo, goback, NULL);
    auto repeatForever = RepeatForever::create(spawn);
    this->runAction(repeatForever);
}

bool Ufo::detectCollision(Sprite *chara)
{
    Rect charaRect = chara->getBoundingBox();
    Rect ufoRect = this->getBoundingBox();

    int charaMinY = charaRect.getMinY();
    int ufoMinY = ufoRect.getMinY();

    int ufoMaxX = ufoRect.getMaxX();
    int ufoMinX = ufoRect.getMinX();
    int charaMaxX = charaRect.getMaxX();
    int charaMinX = charaRect.getMinX();

    if (charaMinY <= ufoMinY &&
        (charaMaxX >= ufoMinX && charaMinX <= ufoMaxX)) {
        return true;
    }

    return false;
}

void Ufo::abductChara(Chara *chara, CallFunc *cb)
{
    chara->isAbducting = true;
    chara->stopAllActions();

    int x = this->getBoundingBox().getMidX();
    int y = this->getBoundingBox().getMidY();

    MoveTo* abduct = MoveTo::create(0.2f, Point(x, y));
    ScaleTo* scale = ScaleTo::create(0.2, 0);
    auto spawn = Spawn::create(abduct, scale, NULL);
    auto seq = Sequence::create(spawn, cb, NULL);
    chara->runAction(seq);
}
