//
//  ForecastPageViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/18/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ForescastTableViewController.h"

@interface ForecastPageViewController : UIPageViewController <UIPageViewControllerDelegate, UIPageViewControllerDataSource>

@property (strong, nonatomic) NSArray<ForescastTableViewController*> *pages;
@property NSInteger initialPage;
@property ForescastTableViewController *forecast1;
@property ForescastTableViewController *forecast2;
@property ForescastTableViewController *forecast3;
@property ForescastTableViewController *forecast4;
@property ForescastTableViewController *forecast5;
@property UIPageControl *pageControl;

@end
