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
