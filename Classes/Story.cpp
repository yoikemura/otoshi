//
//  Story.cpp
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 12/4/16.
//
//

#include "Story.hpp"
#include "HomeScene.h"
#include "SimpleAudioEngine.h"

Scene* Story::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    CCLOG("creatScene on Story Called");
    
    // 'layer' is an autorelease object
    auto layer = Story::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool Story::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    //背景作成
    pBG = Sprite::create("bg_story.png");
    pBG->setPosition(Point(size.width * 0.5, size.height * 0.5));
    
    //画面サイズに合わせる処理
    Size bgSize = pBG->getContentSize();
    float kw, kh;
    if (size.width > bgSize.width) {
        kw = size.width / bgSize.width;
        kh = size.height / bgSize.height;
    } else {
        kw = size.width / bgSize.width;
        kh = size.height / bgSize.height;
    }
    pBG->setScale(kw, kh);
    this->addChild(pBG);
    
    //ITEM表示
    auto startHome = MenuItemImage::create(
                                           "btn_back.png", "btn_back_on.png",
                                           CC_CALLBACK_1(Story::backToHome, this));
    
    Menu* pMenu = Menu::create(startHome, NULL);
    pMenu->setPosition(size.width*0.5, size.height*0.15);
    pMenu->alignItemsHorizontally();
    this->addChild(pMenu);
    
    return true;
}



void Story::backToHome(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
