//
//  PresentationAnimator.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#define left ((int) 1)
#define right ((int) 2)

@interface PresentationAnimator : NSObject <UIViewControllerAnimatedTransitioning>

@property int direction;
@property Boolean isPresentation;

-(instancetype)init: (int) direction : (Boolean) isPresentation;
- (NSTimeInterval)transitionDuration:(id<UIViewControllerContextTransitioning>)transitionContext;
- (void)animateTransition:(id<UIViewControllerContextTransitioning>)transitionContext;

@end
