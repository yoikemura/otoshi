//
//  Config.h
//  Medjed
//
//  Created by JoHazumu on 2014/07/20.
//
//

#ifndef __MaruOtoshi__Config__
#define __MaruOtoshi__Config__

struct CHARA{
    char name[30];
    char fileName[30];
};

// キャラのデータ
static struct CHARA CHARA_DATA[] = {
    {
        "ゴマ-ビィ",
        "goma.png"
    }
};

#endif