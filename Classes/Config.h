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
    char explainFileName[30];
    char description[500];
    int percent;
};

// キャラのデータ
static struct CHARA CHARA_DATA[] = {
    {
        "0001",
        "ゴマ-ビィ",
        "goma.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        80
    },
    {
        "0002",
        "メジェドさん",
        "chara_medjed.png",
        "popup_goma.png",
        "彼はいわゆるエジプトの神様であり、博士の実験とはなんの関係もない、このゲームの製作者が開発した別のゲームの主人公でもあり宣伝のためにこの場に登場しているのだ。",
        20
    },
    {
        "0003",
        "アオゴマ",
        "goma_blue.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0004",
        "ピンクゴマ",
        "goma_pink.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0005",
        "ノッポゴマ",
        "goma_noppo.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0006",
        "ロボゴマ",
        "goma_robot.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0007",
        "ビッグゴマ",
        "goma_big.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0008",
        "オレンジカッパ",
        "orengekappa.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0009",
        "メロンモンキー",
        "melonmonkey.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0010",
        "シャザービー",
        "shadow.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        20
    },
    {
        "0011",
        "アザビー",
        "azaby.png",
        "popup_goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型融合生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
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

// ポップアップ画像
static const char* PLUS1_IMAGE[] = {
    "plus1_yellow.png",
    "plus1_pink.png",
    "plus1_green.png",
    "plus1_blue.png"
};

// フィーバーまでのキャラ数
const int FEVER_NUM = 20;

// レベルデザイン
const float RARE_PROBABILITY_RATE = 0.05;


#endif
