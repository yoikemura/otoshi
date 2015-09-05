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

using namespace cocos2d;

class Ufo : public Sprite
{
public:
    // コンストラクタ
    Ufo();
    
    // 作成
    static Ufo* create();
    bool initWithFile();
    
    // 画面表示
    void show();
    void hide();
    void move();
    void update(float dt);
    
    Size size;

    
    
private:
    Sprite* pSprite;
    
};


#endif /* defined(__MaruOtoshi__Ufo__) */
