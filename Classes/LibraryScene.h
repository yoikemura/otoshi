//
//  LibraryScene.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/05.
//
//

#ifndef __MaruOtoshi__LibraryScene__
#define __MaruOtoshi__LibraryScene__

#include <stdio.h>

using namespace cocos2d;

class LibraryScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void btnToHomeCallback(cocos2d::Ref* pSender);
    
    
    // carousel
    Layer *carouselLayer;
    Point carouselCurrentPoint;

    // Touchイベント用
    Point touchPoint;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);

    
    CREATE_FUNC(LibraryScene);
};

#endif // __Library_SCENE_H__

