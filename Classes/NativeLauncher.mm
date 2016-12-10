//
//  NativeLauncher.m
//  TestPj
//
//  Created by JoHazumu on 2014/05/04.
//
//

#import "NativeLauncher.h"
#import <Twitter/Twitter.h>
#import <Social/Social.h>
#import "MyAlertView.h"
void NativeLauncher::openTweetDialog(const char *$tweet, const char *$image){
    
    NSString *setText = [NSString stringWithUTF8String:$tweet];
    UIImage *image = [UIImage imageNamed:[NSString stringWithFormat:@"%s.png" , $image]];
    NSURL *url = [NSURL URLWithString:@"https://itunes.apple.com/jp/app/go!-go!-mejedoadobencha/id926799955?l=ja&mt=8"];
    
    //投稿用画面のインスタンスを作成
    SLComposeViewController *composeController = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
    
    [composeController setInitialText:setText]; //コメントのセット
    [composeController addImage:image]; //写真のセット
    [composeController addURL:url]; //URLのセット
    
    //投稿が完了したかの確認
    UIViewController *myViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    [composeController setCompletionHandler:^(SLComposeViewControllerResult result) {
        if (result == SLComposeViewControllerResultCancelled) {
            // キャンセルした場合
            NSLog(@"キャンセルしました");
        } else if (result == SLComposeViewControllerResultDone) {
            // 投稿に成功した場合
            NSLog(@"投稿しました");
        }
        [myViewController dismissViewControllerAnimated:YES completion:nil];
    }];
    
    [myViewController presentViewController:composeController animated:YES completion:nil];
}

void NativeLauncher::openFacebookDialog(const char *$tweet, const char *$image){
    
    NSString *setText = [NSString stringWithUTF8String:$tweet];
    UIImage *image = [UIImage imageNamed:[NSString stringWithFormat:@"%s.png" , $image]];
    NSURL *url = [NSURL URLWithString:@"https://itunes.apple.com/jp/app/go!-go!-mejedoadobencha/id926799955?l=ja&mt=8"];
    
    //投稿用画面のインスタンスを作成
    SLComposeViewController *composeController = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
    
    [composeController setInitialText:setText]; //コメントのセット
    [composeController addImage:image]; //写真のセット
    [composeController addURL:url]; //URLのセット
    
    //投稿が完了したかの確認
    UIViewController *myViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
    [composeController setCompletionHandler:^(SLComposeViewControllerResult result) {
        if (result == SLComposeViewControllerResultCancelled) {
            // キャンセルした場合
            NSLog(@"キャンセルしました");
        } else if (result == SLComposeViewControllerResultDone) {
            // 投稿に成功した場合
            NSLog(@"投稿しました");
        }
        [myViewController dismissViewControllerAnimated:YES completion:nil];
    }];
    
    [myViewController presentViewController:composeController animated:YES completion:nil];
}

//画像を投稿
void NativeLauncher::shareWithLine(const char *$image) {
    UIImage *image = [UIImage imageNamed:[NSString stringWithFormat:@"%s.png" , $image]];
    
    UIPasteboard *pasteboard;
    
    //iOS7.0以降では共有のクリップボードしか使えない。その際クリップボードが上書きされてしまうので注意。
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 7.0) {
        pasteboard = [UIPasteboard generalPasteboard];
    } else {
        pasteboard = [UIPasteboard pasteboardWithUniqueName];
    }
    
    [pasteboard setData:UIImagePNGRepresentation(image)
      forPasteboardType:@"public.png"];
    
    NSString *LINEUrlString = [NSString stringWithFormat:@"line://msg/image/%@", pasteboard.name];
    
    
    if ([[UIApplication sharedApplication]
         canOpenURL:[NSURL URLWithString:LINEUrlString]]) {
        [[UIApplication sharedApplication]
         openURL:[NSURL URLWithString:LINEUrlString]];
    } else {
        [MyAlertView alertWithTitle:@"LINEがインストールされていません"
                            message:@"AppStoreを開いてLINEをインストールします。"
                  cancelButtonTitle:@"いいえ"
                       clickedBlock:^(UIAlertView *alertView, NSInteger buttonIndex) {
                            switch (buttonIndex) {
                                case 0://いいえのとき
                                    break;
                                case 1://はいのとき
                                    [[UIApplication sharedApplication]
                                     openURL:[NSURL
                                              URLWithString:@"https://itunes.apple.com/jp/app/line/id443904275?mt=8"]];
                                    break;
                            }
                       } cancelBlock:^(UIAlertView *alertView) {
                           // do something on cancel button
                       } otherButtonTitles:@"はい", nil];
    }
}

