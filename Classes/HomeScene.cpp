#include "HomeScene.h"
#include "GameScene.h"
#include "LibraryScene.h"
#include "SimpleAudioEngine.h"


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
    
    // わかりづらいので色をつける
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    this->addChild(bg);
    
    //音楽設定
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_normal.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_game.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("effect_clicked.mp3");

    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_normal.mp3", true);

    
    // ゲーム画面へ移動ボタン
    auto btnToGame = MenuItemImage::create(
                                           "start.png",
                                           "start.png",
                                           CC_CALLBACK_1(Home::btnToGameCallback, this));
    
    btnToGame->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 50));
    auto menu2 = Menu::create(btnToGame, NULL);
    menu2->setPosition(Vec2::ZERO);
    this->addChild(menu2, 1);
    
    // 図鑑画面へ移動ボタン
    auto btnToLibrary = MenuItemImage::create(
                                           "zukan.png",
                                           "zukan.png",
                                           CC_CALLBACK_1(Home::btnToLibraryCallback, this));
    
    btnToLibrary->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 100));
    auto menu3 = Menu::create(btnToLibrary, NULL);
    menu3->setPosition(Vec2::ZERO);
    this->addChild(menu3, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("月のゴマービィ", "fonts/Osaka.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("logo.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
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

