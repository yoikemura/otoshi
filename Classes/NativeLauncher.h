//
//  NativeLauncher.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/11/01.
//
//

#ifndef NativeLauncher_h
#define NativeLauncher_h
class NativeLauncher{
public:
    static void openTweetDialog(const char* $tweet, const char* $image);
    static void openFacebookDialog(const char* $tweet, const char* $image);
    static void shareWithLine(const char* $image);
};

#endif /* NativeLauncher_h */
