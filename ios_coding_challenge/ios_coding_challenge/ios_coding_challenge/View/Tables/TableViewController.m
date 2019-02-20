//
//  TableViewController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/14/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "TableViewController.h"

@implementation TableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    //TODO: Change this to use data source as a list of searches
    self.dataArray = [[NSMutableArray<NSManagedObject *> alloc] init];
    self.nameArray = [[NSMutableArray<NSString *> alloc] init];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    AppDelegate *appdelegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
    NSManagedObjectContext *context = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
    context = appdelegate.persistentContainer.viewContext;
    NSFetchRequest *request = [NSFetchRequest<NSManagedObject *> fetchRequestWithEntityName:@"Conditions"];
    
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
    UISearchBar *searchBar = [[UISearchBar alloc] init];
    searchBar.barTintColor = UIColor.lightGrayColor;
    searchBar.delegate = self;
    UIToolbar *toolbar = [[UIToolbar alloc] init];
    UIBarButtonItem *doneButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(dismissKeyboard)];
    NSMutableArray *barButtonItems = [[NSMutableArray alloc] init];
    [barButtonItems addObject:doneButton];
    [toolbar sizeToFit];
    [toolbar setItems:barButtonItems];
    searchBar.inputAccessoryView = toolbar;
    return (UITableViewHeaderFooterView*) searchBar;
}

- (void)searchBarSearchButtonClicked:(UISearchBar *)searchBar {
    NSURL *url = [NSURL URLWithString:@"https://qa.foreflight.com/weather/report/"];
    url = [url URLByAppendingPathComponent:searchBar.text];
    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    
    // Configure the session here.
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:configuration];
    
    [[session dataTaskWithURL:url
            completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
                switch (httpResponse.statusCode) {
                    case 200: {
                        NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data
                                                                             options:NSJSONReadingMutableContainers
                                                                               error:&error];
                        NSDictionary *report = [json valueForKey:@"report"];
                        NSDictionary *conditions = [report valueForKey:@"conditions"];
                        NSDictionary *forecast = [report valueForKey:@"forecast"];
                        [self.delegate dataReceived:conditions : forecast];
                        break;
                    }
                    case 404: {
                        dispatch_async(dispatch_get_main_queue(), ^{
                            UIAlertController *alert = [UIAlertController
                                                        alertControllerWithTitle:@"Could not find airport"
                                                        message:@"Please enter a valid code"
                                                        preferredStyle:UIAlertControllerStyleAlert];
                            UIAlertAction *okButton = [UIAlertAction
                                                       actionWithTitle:@"Ok"
                                                       style:UIAlertActionStyleDefault
                                                       handler:^(UIAlertAction * action) {
                                                       }];
                            [alert addAction:okButton];
                            [self presentViewController:alert animated:YES completion:nil];
                        });
                    }
                    case 500: {
                        dispatch_async(dispatch_get_main_queue(), ^{
                            UIAlertController *alert = [UIAlertController
                                                        alertControllerWithTitle:@"No Connection"
                                                        message:@"There is no internet connection. Please try again later"
                                                        preferredStyle:UIAlertControllerStyleAlert];
                            UIAlertAction *okButton = [UIAlertAction
                                                       actionWithTitle:@"Ok"
                                                       style:UIAlertActionStyleDefault
                                                       handler:^(UIAlertAction * action) {
                                                       }];
                            [alert addAction:okButton];
                            [self presentViewController:alert animated:YES completion:nil];
                        });
                    }
                    default:
                        break;
                }
                
         
      }] resume];
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
    [self.conditionDelegate showConditions:object];
}

@end
