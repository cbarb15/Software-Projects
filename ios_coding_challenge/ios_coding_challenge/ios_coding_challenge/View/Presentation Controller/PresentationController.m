//
//  PresentationController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/14/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "PresentationController.h"

@implementation PresentationController
    
@synthesize frameOfPresentedViewInContainerView;

- (instancetype)initWithPresentedViewController:(UIViewController *)presentedViewController presentingViewController:(UIViewController *)presentingViewController : (int) direction {
    self = [super initWithPresentedViewController:presentedViewController presentingViewController:presentingViewController];
    self->direction = direction;
    return self;
}

- (CGRect)frameOfPresentedViewInContainerView {
    CGRect view;
    switch (self->direction) {
        case left:
            view = CGRectMake(self.containerView.frame.origin.x, self.containerView.frame.origin.y, self.containerView.frame.size.width * (3.0/4.0), self.containerView.frame.size.height);
            break;
        case right:
            view = CGRectMake(self.containerView.frame.origin.x + self.containerView.frame.size.width * (1.0/4.0), self.containerView.frame.origin.y, self.containerView.frame.size.width * (3.0/4.0), self.containerView.frame.size.height);
            break;
        default:
            break;
    }
    return view;
}

- (void)presentationTransitionWillBegin {
    dimmingView = [[UIView alloc] initWithFrame:CGRectMake(self.containerView.frame.origin.x,                         self.containerView.frame.origin.y, self.containerView.frame.size.width, self.containerView.frame.size.height)];
    [self.containerView addSubview:dimmingView];
    [dimmingView addSubview: self.presentedViewController.view];
    
    id <UIViewControllerTransitionCoordinator> transitionCoordinator =
    [[self presentingViewController] transitionCoordinator];
    
    [transitionCoordinator animateAlongsideTransition:
     ^(id<UIViewControllerTransitionCoordinatorContext> context) {
         self->dimmingView.backgroundColor = [UIColor.blackColor colorWithAlphaComponent:0.5];
     } completion:nil];
}

- (void)presentationTransitionDidEnd:(BOOL)completed {
    if (!completed) {
        [dimmingView removeFromSuperview];
    }
}
@end
