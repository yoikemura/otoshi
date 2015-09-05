//
//  Enemy.cpp
//  Medjed
//
//  Created by JoHazumu on 2014/07/20.
//
//

#include "Chara.h"
using namespace cocos2d;

Chara::Chara(struct CHARA chara)
{
    charaData = chara;
}

Chara* Chara::create(struct CHARA chara)
{
    Chara *ret = new Chara(chara);
    if (ret && ret->initWithFile(chara.fileName))
    {
        ret->autorelease();
        ret->retain();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool Chara::initWithFile(const char *pFileName)
{
    Sprite::initWithFile(pFileName);
    
    
    return true;
}


const char* Chara::getName()
{
    return charaData.name;
}

void Chara::update(float dt)
{
    
}

void Chara::show()
{
    this->setOpacity(0);
    ActionInterval* action = FadeIn::create(0.5);
    this->runAction(action);
}

void Chara::drop()
{
    int x = this->getPositionX();
    MoveTo* drop = MoveTo::create(1.0f, Point(x, -100));
    RotateTo* rotate = RotateTo::create( 0.0f, 20.0f);
    auto spawn = Spawn::create(rotate, drop, NULL);
    this->runAction(spawn);
}
