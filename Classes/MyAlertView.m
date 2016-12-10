//
//  MyAlertView.m
//  MaruOtoshi
//
//  Created by Hazumu J on 2016/12/11.
//
//

#import <Foundation/Foundation.h>
#import "MyAlertView.h"

@implementation MyAlertView

@synthesize clickedButtonAtIndex = clickedButtonAtIndex_;
@synthesize cancel = cancel_;

+(void) alertWithTitle:(NSString*)title message:(NSString*)message cancelButtonTitle:(NSString*)cancelButtonTitle
          clickedBlock:(void (^)(UIAlertView* alertView, NSInteger buttonIndex))clickedButtonAtIndex cancelBlock:(void (^)(UIAlertView* alertView))cancel
     otherButtonTitles:(NSString*)titles,...;
{
    
    MyAlertView* myAlertView = [[MyAlertView alloc] initWithClickedBlock:clickedButtonAtIndex cancelBlock:cancel];
    
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:title message:message delegate:myAlertView cancelButtonTitle:cancelButtonTitle otherButtonTitles:titles, nil];
    [[alertView autorelease] show];
}

- (void) dealloc;
{
    [clickedButtonAtIndex_ release];
    [clickedButtonAtIndex_ release];
    [super dealloc];
}

-(id)initWithClickedBlock:(void (^)(UIAlertView* alertView, NSInteger buttonIndex))clickedButtonAtIndex cancelBlock:(void (^)(UIAlertView* alertView))cancel;{
    self = [super init];
    if (self) {
        self.clickedButtonAtIndex = clickedButtonAtIndex;
        self.cancel = cancel;
    }
    
    return self;
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;{
    clickedButtonAtIndex_(alertView, buttonIndex);
    [self release];
}

-(void)alertViewCancel:(UIAlertView *)alertView;
{
    cancel_(alertView);
    [self release];
}
@end
