//
//  PresentationAnimator.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "PresentationAnimator.h"

@implementation PresentationAnimator

-(instancetype)init: (int) direction : (Boolean) isPresentation {
    self = [super init];
    self.direction = direction;
    self.isPresentation = isPresentation;
    return self;
}

- (NSTimeInterval)transitionDuration:(nullable id<UIViewControllerContextTransitioning>)transitionContext {
    return 0.4;
}

- (void)animateTransition:(nonnull id<UIViewControllerContextTransitioning>)transitionContext {
    NSString *const key = (self.isPresentation) ? UITransitionContextToViewControllerKey : UITransitionContextFromViewControllerKey;
    UIViewController *const controller = [transitionContext viewControllerForKey:key];
    
    if (self.isPresentation) {
        [transitionContext.containerView addSubview:controller.view];
    }
    
    CGRect const presentedFrame = [transitionContext finalFrameForViewController:controller];
    CGRect dismissedFrame = presentedFrame;
    switch (self.direction) {
        case left:
            dismissedFrame.origin.x = -presentedFrame.size.width;
            break;
        case right:
            dismissedFrame.origin.x = transitionContext.containerView.frame.size.width;
        default:
            break;
    }
    
    CGRect const initialFrame = self.isPresentation ? dismissedFrame : presentedFrame;
    CGRect const finalFrame = self.isPresentation ? presentedFrame : dismissedFrame;

    NSTimeInterval const animationDuration = [self transitionDuration:transitionContext];
    controller.view.frame = initialFrame;
    [UIView animateWithDuration:animationDuration
                     animations:^{
                         controller.view.frame = finalFrame;
                     } completion:^(BOOL finished) {
                         [transitionContext completeTransition:true];
                     }];
}

@end
