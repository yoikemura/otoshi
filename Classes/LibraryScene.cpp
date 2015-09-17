//
//  LibraryScene.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#include "LibraryScene.h"
#include "LibraryManager.h"
#include "HomeScene.h"
#include "config.h"
#include "chara.h"

USING_NS_CC;

Scene* LibraryScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LibraryScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LibraryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithTTF("図鑑画面", "fonts/Osaka.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    auto libraryManager = LibraryManager::getInstance();

    int j = 0;
    for (int i = 0; i < 11; i++) {
        CHARA charaData = CHARA_DATA[i];

        if (libraryManager->hasGotten(charaData.id.c_str())) {
          auto chara = Chara::create(charaData);
          chara->setPosition(Vec2(20 + ((j % 4) * 100), visibleSize.height - 150 - ((j % 4) - 90)));
          this->addChild(chara);
          j++;
        }
    }
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    // ホーム画面へ移動ボタン
    auto btnToHome = MenuItemImage::create(
                                           "back.png",
                                           "back.png",
                                           CC_CALLBACK_1(LibraryScene::btnToHomeCallback, this));
    
    btnToHome->setPosition(Vec2(10,
                                origin.y + btnToHome->getContentSize().height/2));
    auto menu = Menu::create(btnToHome, NULL);
    this->addChild(menu, 1);
    
    return true;
}

void LibraryScene::btnToHomeCallback(Ref* pSender)
{
    Scene* scene = Home::createScene();
    Director::getInstance()->replaceScene(scene);
}
