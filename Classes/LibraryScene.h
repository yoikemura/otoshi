//
//  LibraryScene.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#ifndef __MaruOtoshi__LibraryScene__
#define __MaruOtoshi__LibraryScene__

#include "chara.h"
#include <stdio.h>
#include "Chara.h"


using namespace cocos2d;

class LibraryScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void backToHome(cocos2d::Ref* pSender);

    //Popup用
    LayerColor* overlayLaery;
    Chara* distChara;
    void closePopup(Ref* pSender);
    
    // carousel
    Layer *carouselLayer;
    Point carouselCurrentPoint;
    int current = 0;
    float dist = 0.0f;
    bool moving = false;

    // Touchイベント用
    Point touchPoint;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    
    void next();
    void prev();
    
    
    // 左右矢印
    Sprite* arrowLeft;
    Sprite* arrowRight;
    
    
    // 詳細表示用キャラ配列
    Point detailTouchPoint;
    Vector<Chara*> charas = Vector<Chara*>();
    void showDetail(Chara* chara);
    bool detailMoving = false;
    
    CREATE_FUNC(LibraryScene);
};

#endif // __Library_SCENE_H__

