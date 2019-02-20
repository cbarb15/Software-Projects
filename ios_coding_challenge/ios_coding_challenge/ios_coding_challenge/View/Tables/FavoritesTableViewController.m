//
//  FavoritesTableViewController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/16/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "FavoritesTableViewController.h"

@implementation FavoritesTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    self.dataArray = [[NSMutableArray<NSManagedObject*> alloc] init];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    AppDelegate *appdelegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    NSManagedObjectContext *context = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
    context = appdelegate.persistentContainer.viewContext;
    NSFetchRequest *request = [NSFetchRequest<NSManagedObject *> fetchRequestWithEntityName:@"Favorites"];
    
    NSError* error = nil;
    NSArray* results = [context executeFetchRequest:request error:&error];
    if (!results) {
        NSLog(@"Error fetching Employee objects: %@\n%@", [error localizedDescription], [error userInfo]);
        abort();
    }
    NSMutableArray<NSManagedObject *> *tempArray = [[NSMutableArray alloc] initWithArray:results];
    self.dataArray = tempArray;
}

-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *cellIdentifier = @"cellIdentifier";
    
    UITableViewCell *cell = [self.tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    
    if(cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
        
    }
    NSManagedObject *object = [self.dataArray objectAtIndex:indexPath.row];
    cell.textLabel.text = [object valueForKey:@"name"];
    return cell;
}

-(NSInteger) numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

-(NSInteger) tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dataArray.count;
}

-(UITableViewHeaderFooterView*)tableView: (UITableView*) tableView viewForHeaderInSection:(NSInteger)section {
    UITableViewHeaderFooterView *tableHeader = [[UITableViewHeaderFooterView alloc] initWithFrame:CGRectMake(self.view.frame.origin.x, self.view.frame.origin.y, self.view.frame.size.width, self.view.frame.size.height)];
    UILabel *label = [[UILabel alloc] init];
    label.text = @"Favorites";
    label.font = [UIFont systemFontOfSize:25.0 weight:UIFontWeightBold];
    label.textColor = UIColor.blackColor;
    [tableHeader addSubview:label];
    label.translatesAutoresizingMaskIntoConstraints = NO;
    [label.centerXAnchor constraintEqualToAnchor:tableHeader.centerXAnchor].active = YES;
    [label.centerYAnchor constraintEqualToAnchor:tableHeader.centerYAnchor].active = YES;
    return tableHeader;
}

-(void) dismissKeyboard {
    [self.view endEditing:YES];
}

-(CGFloat)tableView: (UITableView*)tableView heightForHeaderInSection:(NSInteger)section {
    return 55.0;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 55.0;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSManagedObject *object = [self.dataArray objectAtIndex:indexPath.row];
    [self.delegate showFavorites:object];
}

@end
