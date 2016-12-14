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

// ゲーム開始時のゴマビィの数
// const int GOMA_LIMIT      = 70;
const int GOMA_LIMIT = 20;

// リカバリにかかる分
const int GOMA_RECOVERY_MINUTE = 2;

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
    char description[500];
    int rarity;
};

// キャラのデータ
static struct CHARA CHARA_DATA[] = {
    {
        "0001",
        "ゴマビィ",
        "goma.png",
        "仙台名物「萩の月」と「ゴマフアザラシ」の量産型合成生物(キメラ)である。博士の実験台として使われるとこに嫌気が差しているらしい。鳴き声は『キュ〜』。",
        100,
    },
    {
        "0002",
        "メジェドさん",
        "chara_medjed.png",
        "エジプトの神様。博士の実験とはなんの関係もない。このゲームの製作者が開発した別のゲームの主人公でもあり宣伝のためにこの場に登場しているのだ。https://itunes.apple.com/tr/app/go!-go!-mejedoadobencha/id926799955",
        6,
    },
    {
        "0003",
        "アオゴマ",
        "goma_blue.png",
        "「ゴマフアザラシ」と「ラピスラズリ」の合成生物(キメラ)である。好きな映画は 「耳をすませば(1995年)」。つい先週、聖蹟桜ヶ丘に行ってきたところ。青い。",
        6,
    },
    {
        "0004",
        "ピンクゴマ",
        "goma_pink.png",
        "「ゴマフアザラシ」と「アメリカンレッドローズ」の合成生物(キメラ)である彼女は、役者を目指しており日々の努力を惜しまない。演技力は中の上程度。本人より一言「向上心なら誰にも負けないんだから！」",
       12,
    },
    {
        "0005",
        "ノッポゴマ",
        "goma_noppo.png",
        "チェ・ホンマンに憧れ、牛乳を飲み続けた結果身長のみ高くなった。上背はあるが筋肉が追いついていないので一発の重さにかける。目指すところはミルコ・クロコップか!?",
        10,
    },
    {
        "0006",
        "ロボゴマ",
        "goma_robot.png",
        "世田谷に住む独身博士(64)が寂しさを紛らわすために作った人造ゴマフアザラシ。3時間充電で72時間ほど稼働する。博士曰く「iPhoneより長持ち」",
        10,
    },
    {
        "0007",
        "ビッグゴマ",
        "goma_big.png",
        "でかいが正義なのはアメリカだけでなく、ゴマフアザラシの世界でも通用する。ただ大きくなることだけに特化しているためオツムが弱い。",
        30,
    },
    {
        "0008",
        "オレンジカッパ",
        "orengekappa.png",
        "河童とゴマフアザラシが合成するとオレンジ色になることが確認されているが、こいつもその例外ではない。好きな飲物は「なっちゃん」",
        7,
    },
    {
        "0009",
        "メロンモンキー",
        "melonmonkey.png",
        "メロンでメロゥなモンキーGUY!! フルーツ界の神であり、音楽の王様でもある彼は、ゴマフアザラシ界のジャスティン・ビーバー。カーネギーメロン大学卒。",
        10,
    },
    {
        "0010",
        "シャザービー",
        "shadow.png",
        "真っ黒な見た目と裏腹に、好きな言葉は「純血」。仲間を大事にするのはいいところだが、グローバル化した昨今では敬遠されがち。",
        7,
    },
    {
        "0011",
        "アザビー",
        "azaby.png",
        "アザラシとゴマフアザラシのハーフ（アザラシ界では禁断の組み合わせ）。親戚の大反対を押し切って駆け落ちした両親の性欲を憎んでいる。",
        7,
    },
    {
        "0012",
        "雪見ゴマ",
        "yukimigoma.png",
        "みんな大好き雪見だいふく。白くてふわふわのカノジョは、多くのゴマフアザラシから求婚を受けている。",
        5,
    },
    {
        "0013",
        "民衆を導く自由のゴマビィ",
        "goma_free.png",
        "ついに現れた。彼こそが今回の事件の首謀者であり、カリスマ、民衆を導く自由のゴマビィだ！憧れはメタナイト。",
        1,
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

#endif
