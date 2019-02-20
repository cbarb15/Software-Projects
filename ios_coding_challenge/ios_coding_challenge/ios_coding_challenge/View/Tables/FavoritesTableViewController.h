//
//  FavoritesTableViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CoreData/Coredata.h"
#import "AppDelegate.h"

@protocol ShowFavDelegate <NSObject>
@required
-(void)showFavorites: (NSManagedObject*) object;
@end

@interface FavoritesTableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource>

@property (strong, nonatomic) NSMutableArray<NSManagedObject*> *dataArray;
@property (strong, nonatomic) id<ShowFavDelegate> delegate;

@end
