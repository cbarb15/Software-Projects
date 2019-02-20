//
//  ViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/13/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PresentationTransitionController.h"
#import "TableViewController.h"
#import "FavoritesTableViewController.h"
#import "ConditionsTableViewController.h"
#import "ForecastPageViewController.h"
#import "Data.h"
#import "AppDelegate.h"
#define left ((int) 1)
#define right ((int) 2)

@interface ViewController : UIViewController <DataReceivedDelegate, ShowCondDelegate, ShowFavDelegate> {
    
    UIButton *menu;
    UIButton *favorites;
    UIButton *forecast;
    UIButton *saveFavoritesButton;
    UISwipeGestureRecognizer *swipeGesture;
    ConditionsTableViewController *conditionsTableViewController;
    PresentationTransitionController *transitionController;
    TableViewController *tableViewController;
    FavoritesTableViewController *favoritesTableViewController;
}

@property UILabel *airport;
@property UILabel *temp;

-(void)save: (NSDictionary*) conditionsDictionary : (NSDictionary*) forecastDictionary;
-(void)saveToFavorite;
@end

