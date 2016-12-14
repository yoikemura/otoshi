#include "Story.h"
#include "HomeScene.h"
#include "GameScene.h"
#include "LibraryScene.h"
#include "SimpleAudioEngine.h"
#include "LibraryManager.h"


USING_NS_CC;

Scene* Home::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer =Home::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Home::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //音楽設定
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_normal.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_game.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("effect_clicked.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_normal.mp3", true);
    
    // わかりづらいので色をつける
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(bg);

    // ゲーム画面へ移動ボタン
    auto btnToGame = MenuItemImage::create(
                                           "btn_start.png",
                                           "btn_start.png",
                                           CC_CALLBACK_1(Home::btnToGameCallback, this));
    
    btnToGame->setPosition(Vec2(visibleSize.width/2, 120));
    auto menu2 = Menu::create(btnToGame, NULL);
    menu2->setPosition(Vec2::ZERO);
    this->addChild(menu2, 1);
    
    // 図鑑画面へ移動ボタン
    auto btnToLibrary = MenuItemImage::create(
                                           "btn_karte.png",
                                           "btn_karte.png",
                                           CC_CALLBACK_1(Home::btnToLibraryCallback, this));
    
    btnToLibrary->setPosition(Vec2(visibleSize.width/2 , 50));
    auto menu3 = Menu::create(btnToLibrary, NULL);
    menu3->setPosition(Vec2::ZERO);
    this->addChild(menu3, 1);
    
    //Storyのやつ
    auto pHelpItem = MenuItemImage::create(
                                           "btn_help.png",
                                           "btn_help_on.png",
                                           CC_CALLBACK_1(Home::startHelp, this));
    Menu* pMenuHelp = Menu::create(pHelpItem, NULL);
    pMenuHelp->setPosition(visibleSize.width*0.9, visibleSize.height*0.95);
    this->addChild(pMenuHelp);
    

    // ロゴ
    auto sprite = Sprite::create("img_logo.png");
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 80));
    this->addChild(sprite, 0);
    
    char str[16];
    auto libraryManager = LibraryManager::getInstance();
    int rest = libraryManager->calcRestCharaCount();
    sprintf(str,"コンプまで残り%d種類", rest);
    auto completeLabel = Label::createWithSystemFont(str, "HiraKakuProN-W6", 16);
    completeLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 50));
    this->addChild(completeLabel);


    return true;
}


void Home::btnToGameCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm_normal.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_game.mp3", true);
    
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Home::btnToLibraryCallback(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    auto scene = LibraryScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Home::startHelp(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("effect_clicked.mp3");
    CCLOG("start Help on Home");
    Scene* scene = Story::createScene();
    Director::getInstance()->replaceScene(scene);
}

