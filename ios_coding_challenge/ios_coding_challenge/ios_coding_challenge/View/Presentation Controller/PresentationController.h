//
//  PresentationController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/14/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#define left ((int) 1)
#define right ((int) 2)

@interface PresentationController : UIPresentationController {
    UIView *dimmingView;
    int direction;
}

@property(nonatomic, readonly) CGRect frameOfPresentedViewInContainerView;

-(instancetype)initWithPresentedViewController:(UIViewController *)presentedViewController presentingViewController:(UIViewController *)presentingViewController : (int) direction;

-(void) presentationTransitionWillBegin;
- (void)presentationTransitionDidEnd:(BOOL)completed;

@end
