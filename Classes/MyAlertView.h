//
//  MyAlertView.h
//  MaruOtoshi
//
//  Created by Hazumu J on 2016/12/11.
//
//

#ifndef MyAlertView_h
#define MyAlertView_h

#import <Foundation/Foundation.h>

@interface MyAlertView : NSObject<UIAlertViewDelegate>{
    void (^clickedButtonAtIndex_)(UIAlertView* alertView, NSInteger buttonIndex);
    void (^cancel_)(UIAlertView* alertView);
}

@property(nonatomic,copy) void (^clickedButtonAtIndex)(UIAlertView* alertView, NSInteger buttonIndex);
@property(nonatomic,copy) void (^cancel)(UIAlertView* alertView);

+(void) alertWithTitle:(NSString*)title message:(NSString*)message cancelButtonTitle:(NSString*)cancelButtonTitle
          clickedBlock:(void (^)(UIAlertView* alertView, NSInteger buttonIndex))clickedButtonAtIndex cancelBlock:(void (^)(UIAlertView* alertView))cancel
     otherButtonTitles:(NSString*)titles,...;
-(id)initWithClickedBlock:(void (^)(UIAlertView* alertView, NSInteger buttonIndex))clickedButtonAtIndex cancelBlock:(void (^)(UIAlertView* alertView))cancel;

@end

#endif /* MyAlertView_h */
