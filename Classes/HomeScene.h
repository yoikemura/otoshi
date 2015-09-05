#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Home : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // ボタンをおした時のあれ
    void menuCloseCallback(cocos2d::Ref* pSender);
    void btnToGameCallback(cocos2d::Ref* pSender);
    void btnToLibraryCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Home);
};

#endif // __HELLOWORLD_SCENE_H__
