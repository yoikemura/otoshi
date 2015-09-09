//
//  Ufo.h
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#ifndef __MaruOtoshi__Ufo__
#define __MaruOtoshi__Ufo__

#include <stdio.h>
#include "Chara.h"

using namespace cocos2d;

class Ufo : public Sprite
{
public:
    // コンストラクタ
    Ufo();
    
    static Ufo* create();
    bool initWithFile();
    
    void show();
    void hide();
    void move();
    bool detectCollision(Sprite *chara);
    void abductChara(Chara *chara, CallFunc *cb);
};


#endif /* defined(__MaruOtoshi__Ufo__) */
