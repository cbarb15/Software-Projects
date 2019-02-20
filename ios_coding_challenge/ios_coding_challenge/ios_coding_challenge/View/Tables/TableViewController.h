//
//  TableViewController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/14/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CoreData/CoreData.h"
#import "AppDelegate.h"
#import "Data.h"

@protocol DataReceivedDelegate <NSObject>
@required
-(void)dataReceived:(NSDictionary*)conditionsDictionary : (NSDictionary*)forecastDictionary;
@end
@protocol ShowCondDelegate <NSObject>
@required
-(void)showConditions:(NSManagedObject* )object;
@end

@interface TableViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, UISearchBarDelegate, UIAlertViewDelegate> {
    NSURLSession *defaultSession;
    NSURLSessionDataTask *dataTask;
}
@property (strong, nonatomic) id<DataReceivedDelegate> delegate;
@property (strong, nonatomic) id<ShowCondDelegate> conditionDelegate;
//@property (strong, nonatomic) NSMutableArray<NSDictionary *> *dataArray;
@property (strong, nonatomic) NSMutableArray<NSManagedObject *> *dataArray;
@property NSMutableArray<NSString *> *nameArray;
//-save:(Data*)data;
@end
