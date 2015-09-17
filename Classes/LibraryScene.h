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

class LibraryScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void btnToHomeCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(LibraryScene);
};

#endif // __Library_SCENE_H__

