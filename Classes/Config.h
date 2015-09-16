//
//  Config.h
//  Medjed
//
//  Created by JoHazumu on 2014/07/20.
//
//

#ifndef __MaruOtoshi__Config__
#define __MaruOtoshi__Config__

const int EVENT_LOGN      = 0;
const int EVENT_INCREMENT = 1;
const int EVENT_RARE      = 2;

// Slotのデータ
struct SLOT {
    int id;
    char fileName[30];
    char description[50];
};

static struct SLOT SLOT_DATA[] = {
    {
        EVENT_LOGN,
        "slot_long.png",
        "台が伸びる"
    },
    {
        EVENT_INCREMENT,
        "slot_increment.png",
        "ゴマビィ増殖"
    },
    {
        EVENT_RARE,
        "slot_rare.png",
        "レア100%"
    }
};

struct CHARA{
    std::string id;
    char name[30];
    char fileName[30];
    int percent;
};


// キャラのデータ
static struct CHARA CHARA_DATA[] = {
    {
        "0001",
        "ゴマ-ビィ",
        "goma.png",
        80
    },
    {
        "0002",
        "メジェドさん",
        "chara_medjed.png",
        20
    },
    {
        "アオゴマ",
        "goma_blue.png",
        20
    },
    {
        "ピンクゴマ",
        "goma_pink.png",
        20
    },
    {
        "ノッポゴマ",
        "goma_noppo.png",
        20
    },
    {
        "ロボゴマ",
        "goma_robot.png",
        20
    },
    {
        "ビッグゴマ",
        "goma_big.png",
        20
    },
    {
        "オレンジカッパ",
        "orengekappa.png",
        20
    },
    {
        "メロンモンキー",
        "melonmonkey.png",
        20
    },
    {
        "シャザービー",
        "shadow.png",
        20
    },
    {
        "azaby.png",
        "azaby.png",
        20
    }
};

// キャラの初期配置
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

// フィーバーまでのキャラ数
const int FEVER_NUM = 20;

#endif
