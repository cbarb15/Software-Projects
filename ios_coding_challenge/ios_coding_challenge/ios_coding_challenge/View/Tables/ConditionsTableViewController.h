//
//  ConditionsTableViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TableViewCell.h"
#import "TableViewController.h"
#import "CoreData/CoreData.h"

@interface ConditionsTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>

@property NSMutableArray<NSString *> *conditionsData;

@end
