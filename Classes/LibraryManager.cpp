//
//  LibraryManager.cpp
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/12.
//
//

#include "LibraryManager.h"
#include "Config.h"
#include <stdio.h>
using namespace cocos2d;
using namespace rapidjson;

template
<
    typename TYPE,
    int SIZE
>
int arrayLength(const TYPE (&)[SIZE])
{
    return SIZE;
}

const char* kLibrary = "kLibrary";

// 唯一のインスタンスをNULLで初期化
LibraryManager *LibraryManager::instance = NULL;

void LibraryManager::init()
{
    // NOTE: const char*はreturn時に狂う問題あり
    auto defaultLibrary = this->createDefautJSON();
    UserDefault* ud = UserDefault::getInstance();
    std::string libraryJSON = ud->getStringForKey(kLibrary, defaultLibrary);
    this->libraryData = this->parse(libraryJSON);
    
    // キャラ追加時のmigrate処理
    int beforeCharaCount = 0;
    for (rapidjson::Value::ConstMemberIterator itr = this->libraryData.MemberBegin();
         itr != this->libraryData.MemberEnd(); ++itr)
    {
        beforeCharaCount++;
    }
    
    int currentCharaCount = arrayLength(CHARA_DATA);
    if (beforeCharaCount < currentCharaCount) {
        rapidjson::Document::AllocatorType& allocator = this->libraryData.GetAllocator();
        int rest = currentCharaCount - beforeCharaCount;
        for (int i = 0; i < rest; i++) {
            int idx = beforeCharaCount++;
            std::string charaId = CHARA_DATA[idx].id;
            rapidjson::Value n(charaId.c_str(), allocator);
            this->libraryData.AddMember<bool>(n, false, allocator);
        }
    }
}

void LibraryManager::save(const char* charaId)
{
    bool res = this->libraryData[charaId].GetBool();
 
    // ゲット済みじゃなかったら更新する
    if (!res) {
        this->libraryData[charaId] = true;
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        this->libraryData.Accept(writer);
        UserDefault* userDefault = UserDefault::getInstance();
        userDefault->setStringForKey(kLibrary, sb.GetString());
    }
}

bool LibraryManager::hasGotten(const char* charaId)
{
    return this->libraryData[charaId].GetBool();
}

Document LibraryManager::parse(std::string jsonStr)
{
    char* buffer;
    Document document;
    
    buffer = (char *)malloc( strlen(jsonStr.c_str()) + 1 );

    document.Parse(jsonStr.c_str());
    if (document.HasParseError()) {
        // SEE: http://qiita.com/k2ymg/items/eef3b15eaa27a89353ab
        // size_t offset = document.GetErrorOffset();
        // ParseErrorCode code = document.GetParseError();
        // const char* msg = GetParseErrorFunc(code);
        // printf("%d:%d(%s)\n", offset, code);
    }
    // TODO:メモリ解放
    return document;
}

std::string LibraryManager::createDefautJSON()
{
    // JSON生成
    Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    // JSON配列生成
    rapidjson::Value jsonArray(rapidjson::kArrayType);

    int len = sizeof(CHARA_DATA) / sizeof(CHARA_DATA[0]);
    
    // 要素生成
    for (int i = 0; i < len; i++) {
        std::string charaId = CHARA_DATA[i].id;
        // c_strしないとだめ
        rapidjson::Value n(charaId.c_str(), allocator);
        document.AddMember<bool>(n, false, allocator);
    }
    
    // StringBufferへ書き出し
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    // JSON文字列取得
    const char* result = buffer.GetString();
    std::string str = std::string(result);
    
    // string生成
    return str;
}

int LibraryManager::calcRestCharaCount()
{
    int charaCount = arrayLength(CHARA_DATA);
    int sum = 0;
    for (int i = 0; i < charaCount; i++) {
        const char* charaId = CHARA_DATA[i].id.c_str();
        if (this->hasGotten(charaId)) {
            sum++;
        }
    }
    
    return charaCount - sum;
}
