//
//  LibraryManager.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/12.
//
//

#ifndef __MaruOtoshi__LibraryManager__
#define __MaruOtoshi__LibraryManager__

#include <stdio.h>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "json/prettywriter.h"
using namespace cocos2d;
using namespace rapidjson;

// SEE: http://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
class LibraryManager
{
private:
    LibraryManager() {}
public:
    LibraryManager(const LibraryManager&) = delete;
    // LibraryManager& operator(const LibraryManager&) = delete;
    
    static LibraryManager *getInstance()
    {
        return instance;
    }
    
    static void create() {
        if ( !instance ) {
            instance = new LibraryManager;
        }
    }
    
    static void destroy() {
        if ( instance ) {
            delete instance;
            instance= nullptr;
        }
    }
    
    // ゲーム開始時にキャラデータを作る
    void init();
    // 取得したキャラを保存する
    void save(const char*  charId);
    // キャラが取得済みかどうか確認する
    bool hasGotten(const char*  charaId);
    // キャラデータ
    Document libraryData;

private:
    static LibraryManager *instance;
    std::string createDefautJSON();
    Document parse(std::string);
};

#endif /* defined(__MaruOtoshi__LibraryManager__) */
