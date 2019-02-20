//
//  ForecastPageViewController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/18/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "ForecastPageViewController.h"

@implementation ForecastPageViewController


-(instancetype)init {
    self = [super init];
    self.delegate = self;
    self.dataSource = self;
    self.forecast1 = [[ForescastTableViewController alloc] init];
    self.forecast2 = [[ForescastTableViewController alloc] init];
    self.forecast3 = [[ForescastTableViewController alloc] init];
    self.forecast4 = [[ForescastTableViewController alloc] init];
    self.forecast5 = [[ForescastTableViewController alloc] init];
    self.pages = [[NSArray<ForescastTableViewController *> alloc] initWithObjects:self.forecast1, self.forecast2, self.forecast3, self.forecast4, self.forecast5, nil];

    [self setViewControllers: @[self.pages[self.initialPage]] direction:UIPageViewControllerNavigationDirectionForward animated:YES completion:nil];
    self.pageControl = [[UIPageControl alloc] init];
    self.pageControl.currentPageIndicatorTintColor = UIColor.blackColor;
    self.pageControl.pageIndicatorTintColor = UIColor.lightGrayColor;
    self.pageControl.numberOfPages = self.pages.count;
    self.pageControl.currentPage = self.initialPage;
    [self.view addSubview:self.pageControl];
    
    self.pageControl.translatesAutoresizingMaskIntoConstraints = NO;
    [self.pageControl.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor constant:-5.0].active = YES;
    [self.pageControl.widthAnchor constraintEqualToAnchor:self.view.widthAnchor constant:-20.0].active = YES;
    [self.pageControl.heightAnchor constraintEqualToConstant:20.0].active = YES;
    [self.pageControl.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor].active = YES;
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (NSInteger)presentationCountForPageViewController:(UIPageViewController *)pageViewController {
    return 5;
}

- (NSInteger)presentationIndexForPageViewController:(UIPageViewController *)pageViewController {
    return 0;
}
- (ForescastTableViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerBeforeViewController:(UIViewController *)viewController {
    NSInteger const viewControllerIndex = [self.pages indexOfObject:(ForescastTableViewController*)viewController];
    if(viewControllerIndex == 0) {
        return self.pages.lastObject;
    } else {
        return self.pages[viewControllerIndex - 1];
    }
    return nil;
}

- (ForescastTableViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerAfterViewController:(UIViewController *)viewController {
    NSInteger const viewControllerIndex = [self.pages indexOfObject:(ForescastTableViewController*)viewController];
    
    if (viewControllerIndex < self.pages.count - 1) {
            // go to next page in array
        return self.pages[viewControllerIndex + 1];
        } else {
            // wrap to first page in array
            return self.pages.firstObject;
        }
    return nil;

}
- (void)pageViewController:(UIPageViewController *)pageViewController didFinishAnimating:(BOOL)finished previousViewControllers:(NSArray<UIViewController *> *)previousViewControllers transitionCompleted:(BOOL)completed {
    NSInteger viewControllerIndex = [self.pages indexOfObject:pageViewController.viewControllers[0]];
    self.pageControl.currentPage = viewControllerIndex;
}
@end
