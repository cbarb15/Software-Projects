//
//  PresentationTransitionController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "PresentationTransitionController.h"

@implementation PresentationTransitionController

- (UIPresentationController *)presentationControllerForPresentedViewController:(UIViewController *)presented
                                                      presentingViewController:(UIViewController *)presenting
                                                          sourceViewController:(UIViewController *)source {
    UIPresentationController *presentationController = [[PresentationController alloc] initWithPresentedViewController:presented presentingViewController:presenting :self.direction];
    return presentationController;
}

- (id<UIViewControllerAnimatedTransitioning>)animationControllerForPresentedController:(UIViewController *)presented
                                                                  presentingController:(UIViewController *)presenting
                                                                      sourceController:(UIViewController *)source {
    return [[PresentationAnimator alloc] init:self.direction : YES];
}

- (id<UIViewControllerAnimatedTransitioning>)animationControllerForDismissedController:(UIViewController *)dismissed {
    return [[PresentationAnimator alloc] init:self.direction : NO];
}


@end
