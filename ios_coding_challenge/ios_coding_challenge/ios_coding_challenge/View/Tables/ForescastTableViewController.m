//
//  ForescastTableViewController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/18/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "ForescastTableViewController.h"

@implementation ForescastTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    [self.tableView registerNib:[UINib nibWithNibName:@"ForecastTableViewCell" bundle:nil] forCellReuseIdentifier:@"forecastCell"];
    self.forecastData = [[NSMutableArray<NSString *> alloc] init];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 6;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    UIView *view = [[UIView alloc] init];
    [self.tableView addSubview:view];
    view.translatesAutoresizingMaskIntoConstraints = NO;
    [view.widthAnchor constraintEqualToAnchor:self.tableView.widthAnchor].active = YES;
    [view.heightAnchor constraintEqualToAnchor:self.tableView.heightAnchor].active = YES;
    UILabel *label = [[UILabel alloc] init];
    [view addSubview:label];
    label.translatesAutoresizingMaskIntoConstraints = NO;
    [label.centerXAnchor constraintEqualToAnchor:view.centerXAnchor].active = YES;
    label.text = @"Forecast";
    label.font = [UIFont systemFontOfSize:25.0 weight:UIFontWeightBold];
    return view;
    
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 35.0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    ForecastTableViewCell *cell = [self.tableView dequeueReusableCellWithIdentifier:@"forecastCell" forIndexPath:indexPath];
    
    if(self.forecastData.count > 0) {
        switch (indexPath.row) {
            case 0:
                cell.leftTitle.text = @"Elevation";
                cell.leftLabel.text = [self.forecastData[indexPath.row * 2] stringByAppendingString:@" ft"];
                cell.rightTitle.text = @"Dew Point";
                cell.rightLabel.text = [self.forecastData[indexPath.row * 2 + 1] stringByAppendingString:@" C"];
                break;
            case 1:
                cell.leftTitle.text = @"Pressure";
                cell.leftLabel.text = [self.forecastData[indexPath.row * 2] stringByAppendingString:@" Hg"];
                cell.rightTitle.text = @"Density Alt.";
                cell.rightLabel.text = [self.forecastData[indexPath.row * 2 + 1] stringByAppendingString:@" ft"];
                break;
            case 2:
                cell.leftTitle.text = @"FlightRules";
                cell.leftLabel.text = self.forecastData[indexPath.row * 2];
                cell.rightTitle.text = @"Relative Humidity";
                cell.rightLabel.text = [self.forecastData[indexPath.row * 2 + 1] stringByAppendingString:@"%"];
                break;
            case 3:
                cell.leftTitle.text = @"Cloud Coverage";
                cell.leftLabel.text = self.forecastData[indexPath.row * 2];
                cell.rightTitle.text = @"Altitude";
                cell.rightLabel.text = [self.forecastData[indexPath.row * 2 + 1] stringByAppendingString:@" ft"];
                break;
            case 4:
                cell.leftTitle.text = @"Visiblity";
                cell.leftLabel.text = [self.forecastData[indexPath.row * 2] stringByAppendingString:@" Sm"];
                cell.rightTitle.text = @"Prevailing Vis.";
                cell.rightLabel.text = [self.forecastData[indexPath.row * 2 + 1] stringByAppendingString:@" C"];
                break;
            case 5:
                cell.leftTitle.text = @"Wind Speed";
                cell.leftLabel.text = [self.forecastData[indexPath.row * 2] stringByAppendingString:@" Kts"];
                cell.rightTitle.text = @"Variable";
                if(self.forecastData[indexPath.row * 2 + 1]) {
                    cell.rightLabel.text = @"Yes";
                } else {
                    cell.rightLabel.text = @"No";
                }
                break;
            default:
                break;
        }
    }
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 75.0;
}

@end
