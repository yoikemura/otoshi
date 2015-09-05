//
//  Slot.h
//  MaruOtoshi
//
//  Created by Hayato Hoshikawa on 9/5/15.
//
//

#ifndef __MaruOtoshi__Slot__
#define __MaruOtoshi__Slot__

#include <stdio.h>
using namespace cocos2d;

class Slot : public Sprite
{
public:
    // コンストラクタ
    Slot();
    
    // 作成
    static Slot* create();
    bool initWithFile();
    
    // 画面表示
    void show();
    void hide();
    void update(float dt);
    
    Size size;
    Size slot_size;

    
private:
    Sprite* pSprite;
    
};

#endif /* defined(__MaruOtoshi__Slot__) */
