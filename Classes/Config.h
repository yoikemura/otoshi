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
    char name[50];
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
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型合成生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        22
    },
    {
        "0002",
        "メジェドさん",
        "chara_medjed.png",
        "popup_goma.png",
        "彼はいわゆるエジプトの神様であり、博士の実験とはなんの関係もない。このゲームの製作者が開発した別のゲームの主人公でもあり宣伝のためにこの場に登場しているのだ。https://itunes.apple.com/tr/app/go!-go!-mejedoadobencha/id926799955",
        12
    },
    {
        "0003",
        "アオゴマ",
        "goma_blue.png",
        "popup_goma.png",
        "「ゴマフアザラシ」と「ラピスラズリ」の合成生物(キメラ)である。好きな映画は 「耳をすませば(1995年)」。つい先週、聖蹟桜ヶ丘に行ってきたところ。青い。",
        28
    },
    {
        "0004",
        "ピンクゴマ",
        "goma_pink.png",
        "popup_goma.png",
        "「ゴマフアザラシ」と「アメリカンレッドローズ」の合成生物(キメラ)である彼は、役者を目指してをり日々努力を惜しまない。演技力は中の上程度でるは向上心なら誰にも負けちゃいなんだから!",
        23
    },
    {
        "0005",
        "ノッポゴマ",
        "goma_noppo.png",
        "popup_goma.png",
        "チェ・ホンマンに憧れ、牛乳を飲み続けた結果身長のみ高くなった。上背はあるが筋肉が追いついていないので一発の重さにかける。目指すところはミルコ・クロコップか!?",
        18
    },
    {
        "0006",
        "ロボゴマ",
        "goma_robot.png",
        "popup_goma.png",
        "世田谷に住む独身博士(64)が寂しさを紛らわすために作った人造アザラシ。3時間充電で32時間ほど稼働する。博士曰く「iPhoneより長持ち」",
        15
    },
    {
        "0007",
        "ビッグゴマ",
        "goma_big.png",
        "popup_goma.png",
        "でかいが正義なのはアメリカだけでなく、アザラシの世界でも通用する。ただ大きくなることだけに特化しているためオツムが弱い。",
        14
    },
    {
        "0008",
        "オレンジカッパ",
        "orengekappa.png",
        "popup_goma.png",
        "河童とアザラシが合成するとオレンジ色になることが確認されているが、こいつもその例外ではない。好きな飲物は「なっちゃん」",
        12
    },
    {
        "0009",
        "メロンモンキー",
        "melonmonkey.png",
        "popup_goma.png",
        "メロンでメロゥなモンキーGUY. フルーツの王様であり、音楽の王様でもある彼はゴマアザラシ界のジャスティン・ビーバー。",
        10
    },
    {
        "0010",
        "シャザービー",
        "shadow.png",
        "popup_goma.png",
        "真っ黒な見た目と裏腹に、好きな言葉は「純血」。仲間を大事にするのはいいとこだが、グローバル化した昨今では敬遠されがち。",
        10
    },
    {
        "0011",
        "アザビー",
        "azaby.png",
        "popup_goma.png",
        "あざといやつが現れた。アザービーゴーゴー。",
        7
    },
    {
        "0012",
        "雪見ゴマ",
        "yukimigoma.png",
        "popup_goma.png",
        "みんな大好き雪見だいふく。",
        12
    },
    {
        "0013",
        "民衆を導きし自由のごまびぃ",
        "goma_free.png",
        "popup_goma.png",
        "ついに現れた。彼こそが今回の事件の首謀者であり、カリスマ、民衆を導きし自由のごまびぃだ！",
        1
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
