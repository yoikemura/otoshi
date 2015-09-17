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

// 唯一のインスタンスをNULLで初期化
LibraryManager *LibraryManager::instance = NULL;

void LibraryManager::init()
{
    log("libraryManager init");
    UserDefault* userDefault = UserDefault::getInstance();
    const char* key = "library";
    // NOTE: const char*はreturn時に狂う問題あり
    auto defaultLibrary = this->createDefautJSON();
    log("defaultLibrary %s", defaultLibrary.c_str());
    std::string libraryJSON = userDefault->getStringForKey(key, defaultLibrary);
    log("libraryJSON %s", libraryJSON.c_str());
    this->libraryData = this->parse(libraryJSON);
}

void LibraryManager::save(const char* charaId)
{
    log("get chara with saving charaId: %s", charaId);
    bool res = this->libraryData[charaId].GetBool();
 
    // ゲット済みじゃなかったら更新する
    if (!res) {
        this->libraryData[charaId] = true;
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        this->libraryData.Accept(writer);
        log("保存する！%s", sb.GetString());
        UserDefault* userDefault = UserDefault::getInstance();
        userDefault->setStringForKey("library", sb.GetString());
    }
}

bool LibraryManager::hasGotten(const char* charaId)
{
    log("call hasGotten charaId: %s", charaId);
    bool res = this->libraryData[charaId].GetBool();
    return res;
}

Document LibraryManager::parse(std::string jsonStr)
{
    char* buffer;
    Document document;
    
    buffer = (char *)malloc( strlen(jsonStr.c_str()) + 1 );
    printf(buffer, "%s", jsonStr.c_str());

    document.Parse(jsonStr.c_str());
    if (document.HasParseError()) {
        log("パースエラー！！！！！");
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
    log("create %s", str.c_str());
    
    // string生成
    return str;
}
