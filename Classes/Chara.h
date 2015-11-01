#ifndef __MaruOtoshi__Chara__
#define __MaruOtoshi__Chara__

#include <iostream>
#include "cocos2d.h"
#include "Config.h"
using namespace cocos2d;

class Chara : public Sprite
{
public:
    Chara(struct CHARA);
    
    struct CHARA charaData;
    
    // ステート
    // 上の台にいる
    bool isUpperTable = false;
    // 下の台にいる
    bool isLowerTable = false;
    // 落ちている途中
    bool isDroppingFromUpperTable = false;
    // 上の台から落ちている途中
    bool isDropping = false;
    // 現れ中
    bool isShowing = false;
    // UFOにさらわれ中
    bool isAbducting = false;

    // 作成
    static Chara* create(struct CHARA);
    bool initWithFile(const char *pFileName);
    
    void update(float dt);

    // 画面に現れる
    void show(Vec2 vec);
    // 下に落ちる
    void drop();
    void dropFromUpperTable();
    
    // アクセサ
    const char* getName();
    std::string getId();
    const char* getExplainFimeName();
    const char* getDescription();
};

#endif
