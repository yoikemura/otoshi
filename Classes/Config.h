//
//  Config.h
//  Medjed
//
//  Created by JoHazumu on 2014/07/20.
//
//

#ifndef __MaruOtoshi__Config__
#define __MaruOtoshi__Config__

//Slot のデータ
struct SLOT {
    int id;
    char fileName[30];
    char name[50];
};


static struct SLOT SLOT_DATA[] = {
    {
      1,
        "slot_fuck.png",
        "ファッキュー！！！"
    },
    {
        2,
        "slot_jap.png",
        "ジャップーー！！！"
    },
    {
        3,
        "slot_jp.png",
        "ジャパーンー！！"
    },
    {
        4,
        "slot_fever.png",
        "フィーバー！！！"
    }
};

struct CHARA{
    char name[30];
    char fileName[30];
    int percent;
};


// キャラのデータ
static struct CHARA CHARA_DATA[] = {
    {
        "ゴマ-ビィ",
        "goma.png",
        80
    },
    {
        "メジェドさん",
        "chara_medjed.png",
        20
    }
    
};

static int DEFAULT_CHARA_MAP[] = {
  281,
  280,
  273,
  270,
  241,
  209,
  196,
  186,
  183,
  182,
  182,
  175,
  167,
  153,
  145,
  137,
  127,
  125,
  125,
  123,
  120,
  119,
  111,
  100,
  92,
  91,
  72,
  69,
  50,
  47
};

#endif
