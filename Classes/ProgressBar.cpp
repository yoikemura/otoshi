//
//  ProgressBar.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/18.
//
//

#include "ProgressBar.h"
#include "Config.h"
using namespace cocos2d;

ProgressBar::ProgressBar()
{
}

ProgressBar::~ProgressBar()
{
}

bool ProgressBar::init()
{
    if (!Layer::init()) {
        return false;
    }

    // NOTE: とりあえずBarの幅は115とおく
    this->bar = Sprite::create();
    this->bar->setTextureRect(Rect(0, 0, 115, 8));
    this->bar->setColor(Color3B(237, 74, 101));
    this->bar->setAnchorPoint(Vec2(0, 0));
    this->addChild(bar);
    
    auto progressBar = Sprite::create("progress_bar.png");
    this->addChild(progressBar);

    // bar初期化
    // 最初は幅0
    this->bar->setPosition(Vec2(-88, -4));
    this->bar->setScale(0.0f, 1.0f);
    
    return true;
}

void ProgressBar::update(float dt)
{
    
}

void ProgressBar::setWidth(float rate)
{
    auto action = ScaleTo::create(0.2f, rate, 1.0f);
    bar->runAction(action);
}
