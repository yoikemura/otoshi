//
//  Enemy.cpp
//  Medjed
//
//  Created by JoHazumu on 2014/07/20.
//
//

#include "Chara.h"
#include "SimpleAudioEngine.h"
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

const char* Chara::getFileName()
{
    return charaData.fileName;
}

const char* Chara::getDescription()
{
    return charaData.description;
}

const char* Chara::getMix1()
{
    return charaData.mix1;
}

const char* Chara::getMix2()
{
    return charaData.mix2;
}

const char* Chara::getCry()
{
    return charaData.cry;
}

std::string Chara::getId()
{
    return charaData.id;
}

const int Chara::getRarity()
{
    return charaData.rarity;
}

const int Chara::getLike()
{
    return charaData.like;
}

void Chara::update(float dt)
{
}

void Chara::show(Vec2 vec)
{
    this->isShowing = true;
    this->setPosition(vec);
    MoveTo* fallDown =  MoveTo::create(0.1f, Point(vec.x, vec.y - 40));
    auto cb = CallFunc::create([this](){
        // あとから追加されたやつは絶対上のテーブル
        this->isUpperTable = true;
        this->isShowing = false;
    });
    auto seq = Sequence::create(fallDown, cb, NULL);
    this->runAction(seq);
}

void Chara::dropFromUpperTable()
{
    this->isDroppingFromUpperTable = true;
    int x = this->getPositionX();
    int y = this->getPositionY();
    MoveTo* drop = MoveTo::create(0.1f, Point(x, y-15));
    auto cb = CallFunc::create([this](){
        this->isDroppingFromUpperTable = false;
    });
    this->runAction(Sequence::create(drop, cb, NULL));
}

void Chara::drop()
{
    int x = this->getPositionX();
    MoveTo* drop = MoveTo::create(0.2f, Point(x, -100));
    RotateTo* rotate = RotateTo::create( 0.0f, 20.0f);
    // TintTo* tint = TintTo::create(1.0f, 255, 0, 0); 
    auto cb = CallFunc::create([](){
      CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_drop.mp3");
    });
    auto spawn = Spawn::create(rotate, drop, NULL);
    this->runAction(Sequence::create(
                                     // tint,
                                     cb,
                                     spawn,
                                     NULL
                                     ));
}
