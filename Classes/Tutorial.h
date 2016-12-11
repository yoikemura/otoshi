//
//  Tutorial.hpp
//  MaruOtoshi
//
//  Created by Hazumu J on 2016/12/11.
//
//

#ifndef Tutorial_hpp
#define Tutorial_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Config.h"

using namespace cocos2d;

class Tutorial : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void backToHome(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Tutorial);
};

#endif /* Tutorial_hpp */
