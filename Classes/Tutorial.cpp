//
//  Tutorial.cpp
//  MaruOtoshi
//
//  Created by Hazumu J on 2016/12/11.
//
//

#include "Tutorial.h"
#include "HomeScene.h"
#include "SimpleAudioEngine.h"

Scene* Tutorial::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Tutorial::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool Tutorial::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    
    //背景作成
    auto pBG = Sprite::create("bg_story.png");
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
    
    // ゲーム画面へ移動ボタン
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto btnToHome = MenuItemImage::create("btn_start.png",
                                           "btn_start.png",
                                           CC_CALLBACK_1(Tutorial::backToHome, this));
    Menu* pMenu = Menu::create(btnToHome, NULL);
    pMenu->setPosition(Vec2(visibleSize.width/2, 100));
    this->addChild(pMenu);
    
    return true;
}

void Tutorial::backToHome(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
