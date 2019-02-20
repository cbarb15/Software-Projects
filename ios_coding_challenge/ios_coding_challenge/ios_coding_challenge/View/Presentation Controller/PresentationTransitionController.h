//
//  PresentationTransitionController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PresentationController.h"
#import "PresentationAnimator.h"

@interface PresentationTransitionController : NSObject <UIViewControllerTransitioningDelegate>

@property int direction;

- (UIPresentationController *)presentationControllerForPresentedViewController:(UIViewController *)presented
                                                      presentingViewController:(UIViewController *)presenting
                                                          sourceViewController:(UIViewController *)source;

- (id<UIViewControllerAnimatedTransitioning>)animationControllerForPresentedController:(UIViewController *)presented
                                                                  presentingController:(UIViewController *)presenting
                                                                      sourceController:(UIViewController *)source;

- (id<UIViewControllerAnimatedTransitioning>)animationControllerForDismissedController:(UIViewController *)dismissed;

@end
