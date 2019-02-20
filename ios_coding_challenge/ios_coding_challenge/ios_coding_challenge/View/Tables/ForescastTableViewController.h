//
//  ForescastTableViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/18/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ForecastTableViewCell.h"

@interface ForescastTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>

@property NSMutableArray<NSString *> *forecastData;

@end
