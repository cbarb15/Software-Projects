//
//  ViewController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/13/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    [self setUpViews];
}

-(void)showTable {
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(dismissTable)];
    swipeGesture.direction = UISwipeGestureRecognizerDirectionRight;
    [transitionController setDirection: right];
    tableViewController = [[TableViewController alloc] init];
    tableViewController.delegate = self;
    tableViewController.conditionDelegate = self;
    [tableViewController.view addGestureRecognizer:swipeGesture];
    tableViewController.transitioningDelegate = transitionController;
    tableViewController.modalPresentationStyle = UIModalPresentationCustom;
    [self presentViewController:tableViewController animated:YES completion:nil];
}

- (void)dismissTable {
    [self dismissViewControllerAnimated:true completion:nil];
}

- (void) showFavorites {
    swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(dismissTable)];
    swipeGesture.direction = UISwipeGestureRecognizerDirectionLeft;
    [transitionController setDirection: left];
    favoritesTableViewController = [[FavoritesTableViewController alloc] init];
    favoritesTableViewController.delegate = self;
    [favoritesTableViewController.view addGestureRecognizer:swipeGesture];
    favoritesTableViewController.transitioningDelegate = transitionController;
    favoritesTableViewController.modalPresentationStyle = UIModalPresentationCustom;
    [self presentViewController:favoritesTableViewController animated:YES completion:nil];
}

- (void) setUpViews {
    menu = [[UIButton alloc] init];
    UIImage *image = [UIImage imageNamed:@"menu"];
    menu.translatesAutoresizingMaskIntoConstraints = NO;
    [menu setImage: image forState:UIControlStateNormal];
    [self.view addSubview:menu];
    [menu.topAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.topAnchor constant: 15.0].active = YES;
    [menu.trailingAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.trailingAnchor constant: -25.0].active = YES;
    [menu addTarget:self action:@selector(showTable) forControlEvents:UIControlEventTouchUpInside];
    transitionController = [[PresentationTransitionController alloc] init];
    
    //Favorites Button
    favorites = [[UIButton alloc] init];
    [favorites setTitle:@"Favorites" forState:UIControlStateNormal];
    [favorites setTitleColor:UIColor.blueColor forState:UIControlStateNormal];
    favorites.translatesAutoresizingMaskIntoConstraints = NO;
    [favorites addTarget:self action:@selector(showFavorites) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:favorites];
    [favorites.topAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.topAnchor constant: 15.0].active = YES;
    [favorites.leadingAnchor constraintEqualToAnchor:self.view.safeAreaLayoutGuide.leadingAnchor constant: 25.0].active = YES;
    
    //Airport label
    self.airport = [[UILabel alloc] init];
    [self.view addSubview:self.airport];
    self.airport.font = [UIFont systemFontOfSize:35.0 weight:UIFontWeightBold];
    self.airport.translatesAutoresizingMaskIntoConstraints = NO;
    [self.airport.topAnchor constraintEqualToAnchor:self.view.topAnchor constant:85.0].active = YES;
    [self.airport.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor].active = YES;
    
    //Temp label
    self.temp = [[UILabel alloc] init];
    [self.view addSubview:self.temp];
    self.temp.font = [UIFont systemFontOfSize:45.0];
    self.temp.translatesAutoresizingMaskIntoConstraints = NO;
    [self.temp.topAnchor constraintEqualToAnchor:self.airport.bottomAnchor constant:5.0].active = YES;
    [self.temp.centerXAnchor constraintEqualToAnchor:self.airport.centerXAnchor].active = YES;
    
    conditionsTableViewController = [[ConditionsTableViewController alloc] init];
    [self.view addSubview:conditionsTableViewController.view];
    conditionsTableViewController.tableView.scrollEnabled = NO;
    conditionsTableViewController.view.translatesAutoresizingMaskIntoConstraints = NO;
    [conditionsTableViewController.view.topAnchor constraintEqualToAnchor:self.temp.bottomAnchor constant:35].active = YES;
    [conditionsTableViewController.view.widthAnchor constraintEqualToAnchor:self.view.widthAnchor multiplier:1.0].active = YES;
    [conditionsTableViewController.view.heightAnchor constraintEqualToAnchor:self.view.heightAnchor multiplier:0.55].active = YES;
    
    //Forecast Button
    forecast = [[UIButton alloc] init];
    [self.view addSubview:forecast];
    [forecast setTitle:@"Forecast" forState:UIControlStateNormal];
    [forecast setTitleColor:UIColor.blueColor forState:UIControlStateNormal];
    [forecast addTarget:self action:@selector(showForecast) forControlEvents:UIControlEventTouchUpInside];
    forecast.translatesAutoresizingMaskIntoConstraints = NO;
    [forecast.topAnchor constraintEqualToAnchor:conditionsTableViewController.view.bottomAnchor constant:25.0].active = YES;
    [forecast.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor].active = YES;
    [forecast.widthAnchor constraintEqualToConstant:150.0].active = YES;
    
    saveFavoritesButton = [[UIButton alloc] init];
    [self.view addSubview:saveFavoritesButton];
    [saveFavoritesButton setImage:[UIImage imageNamed:@"favorites"] forState:UIControlStateNormal];
    [saveFavoritesButton addTarget:self action:@selector(saveToFavorite) forControlEvents:UIControlEventTouchUpInside];
    saveFavoritesButton.translatesAutoresizingMaskIntoConstraints = NO;
    [saveFavoritesButton.topAnchor constraintEqualToAnchor:conditionsTableViewController.view.bottomAnchor constant:30.0].active = YES;
    [saveFavoritesButton.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor constant:35.0].active = YES;
}

- (void) showForecast {
    ForecastPageViewController *forecastPageViewController = [[ForecastPageViewController alloc] init];
    [self presentViewController:forecastPageViewController animated:YES completion:nil];
}

- (void)dataReceived:(NSDictionary *)conditionsDictionary : (NSDictionary*)forecastDictionary {
    [conditionsTableViewController.conditionsData removeAllObjects];

    [self save:conditionsDictionary :forecastDictionary];
    [self createForecastDictionary:forecastDictionary];
    
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"elevationFt"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"dewpointC"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"pressureHg"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"densityAltitudeFt"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[conditionsDictionary valueForKey:@"flightRules"]];
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"relativeHumidity"] stringValue]];
    NSDictionary* cloudLayers = [conditionsDictionary valueForKey:@"cloudLayers"];
    [conditionsTableViewController.conditionsData addObject:[cloudLayers valueForKey:@"coverage"][0]];
    id alt = [cloudLayers valueForKey:@"altitudeFt"][0];
    [conditionsTableViewController.conditionsData addObject:[alt stringValue]];
    NSDictionary* visibility = [conditionsDictionary valueForKey:@"visibility"];
    [conditionsTableViewController.conditionsData addObject:[[visibility valueForKey:@"distanceSm"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[[visibility valueForKey:@"prevailingVisSm"] stringValue]];
    NSDictionary* wind = [conditionsDictionary valueForKey:@"wind"];
    [conditionsTableViewController.conditionsData addObject:[[wind valueForKey:@"speedKts"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[[wind valueForKey:@"variable"] stringValue]];
    [conditionsTableViewController.conditionsData addObject:[conditionsDictionary valueForKey:@"ident"]];
    [conditionsTableViewController.conditionsData addObject:[[conditionsDictionary valueForKey:@"tempC"] stringValue]];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.airport.text = [conditionsDictionary valueForKey:@"ident"];
        self.temp.text = [[[conditionsDictionary valueForKey:@"tempC"] stringValue] stringByAppendingString:@" C"];
        [self->tableViewController dismissViewControllerAnimated:true completion:^{
            [self->conditionsTableViewController.tableView reloadData];
        }];
    });
}

- (void)showConditions:(NSManagedObject *)object {
    [conditionsTableViewController.conditionsData removeAllObjects];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"elevation"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"dewPoint"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"pressure"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"altitude"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"flightRules"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"relativeHumidity"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"cloudCoverage"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"altitude"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"visibility"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"prevailingVision"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"windSpeed"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"variableWind"]];

    dispatch_async(dispatch_get_main_queue(), ^{
        self.airport.text = [object valueForKey:@"name"];
        self.temp.text = [[object valueForKey:@"tempC"] stringByAppendingString:@" C"];
        [self->tableViewController dismissViewControllerAnimated:true completion:^{
            [self->conditionsTableViewController.tableView reloadData];
        }];
    });
}

- (void)showFavorites:(NSManagedObject *)object {
    [conditionsTableViewController.conditionsData removeAllObjects];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"elevation"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"dewPoint"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"pressure"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"altitude"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"flightRules"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"relativeHumidity"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"cloudCoverage"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"altitude"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"visibility"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"prevailingVision"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"windSpeed"]];
    [conditionsTableViewController.conditionsData addObject:[object valueForKey:@"variableWind"]];
    
    self.airport.text = [object valueForKey:@"name"];
    self.temp.text = [[object valueForKey:@"tempC"] stringByAppendingString:@" C"];
    [self->favoritesTableViewController dismissViewControllerAnimated:true completion:^{
        [self->conditionsTableViewController.tableView reloadData];
    }];
}

//Get data from the query save it to a data object
//save that object to the disk overriting if there was already and object there for a certain airport
//Only keeping most recent record for each airport.
- (void)save:(NSDictionary *)conditionsDictionary : (NSDictionary*)forecastDictionary {
    Data *data = [[Data alloc] init];
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate *appdelegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        NSManagedObjectContext *context = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
        context = appdelegate.persistentContainer.viewContext;
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"Conditions" inManagedObjectContext:context];
        NSManagedObject *object = [[NSManagedObject alloc] initWithEntity:entity insertIntoManagedObjectContext:context];
        
        [data setElevation:[[conditionsDictionary valueForKey:@"elevationFt"] stringValue]];
        [data setDewPoint: [[conditionsDictionary valueForKey:@"dewpointC"] stringValue]];
        [data setPressure:[[conditionsDictionary valueForKey:@"pressureHg"] stringValue]];
        [data setDensityAlt:[[conditionsDictionary valueForKey:@"densityAltitudeFt"] stringValue]];
        [data setFlightRules:[conditionsDictionary valueForKey:@"flightRules"]];
        [data setRelativeHumidity:[[conditionsDictionary valueForKey:@"relativeHumidity"] stringValue]];
        NSDictionary* cloudLayers = [conditionsDictionary valueForKey:@"cloudLayers"];
        [data setCloudCoverage:[cloudLayers valueForKey:@"coverage"][0]];
        id alt = [cloudLayers valueForKey:@"altitudeFt"][0];
        [data setAltitude:[alt stringValue]];
        NSDictionary* visibility = [conditionsDictionary valueForKey:@"visibility"];
        [data setVisibility:[[visibility valueForKey:@"distanceSm"] stringValue]];
        [data setPrevailingVisibility:[[visibility valueForKey:@"prevailingVisSm"] stringValue]];
        NSDictionary* wind = [conditionsDictionary valueForKey:@"wind"];
        [data setWindSpeed:[[wind valueForKey:@"speedKts"] stringValue]];
        [data setVariable:[[wind valueForKey:@"variable"] stringValue]];
        
        if(![self->tableViewController.nameArray containsObject:[conditionsDictionary valueForKey:@"ident"]]) {
            [object setValue:data.elevation forKey:@"elevation"];
            [object setValue:data.dewPoint  forKey:@"dewPoint"];
            [object setValue:data.pressure forKey:@"pressure"];
            [object setValue:data.densityAlt forKey:@"densityAltitude"];
            [object setValue:data.flightRules forKey:@"flightRules"];
            [object setValue:data.relativeHumidity forKey:@"relativeHumidity"];
            [object setValue:data.cloudCoverage forKey:@"cloudCoverage"];
            [object setValue:data.altitude forKey:@"altitude"];
            [object setValue:data.visibility forKey:@"visibility"];
            [object setValue:data.prevailingVisibility forKey:@"prevailingVision"];
            [object setValue:data.windSpeed forKey:@"windSpeed"];
            [object setValue:data.variable forKey:@"variableWind"];
            [object setValue:[conditionsDictionary valueForKey:@"ident"] forKey:@"name"];
            [object setValue:[[conditionsDictionary valueForKey:@"tempC"] stringValue] forKey:@"tempC"];
        } else {
            NSLog(@"don't save");
        }
        
        
        NSError *error = nil;
         if ([context save:&error] == NO) {
             NSAssert(NO, @"Error saving context: %@\n%@", [error localizedDescription], [error userInfo]);
         } else {
             [self->tableViewController.dataArray addObject:object];
             [self->tableViewController.nameArray addObject:[conditionsDictionary valueForKey:@"ident"]];
         }
    });
}

- (void)saveToFavorite {
    dispatch_async(dispatch_get_main_queue(), ^{
        AppDelegate *appdelegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        NSManagedObjectContext *context = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
        context = appdelegate.persistentContainer.viewContext;
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"Favorites" inManagedObjectContext:context];
        NSManagedObject *object = [[NSManagedObject alloc] initWithEntity:entity insertIntoManagedObjectContext:context];
        
        [object setValue:self->conditionsTableViewController.conditionsData[0] forKey:@"elevation"];
        [object setValue:self->conditionsTableViewController.conditionsData[1]  forKey:@"dewPoint"];
        [object setValue:self->conditionsTableViewController.conditionsData[2] forKey:@"pressure"];
        [object setValue:self->conditionsTableViewController.conditionsData[3] forKey:@"densityAltitude"];
        [object setValue:self->conditionsTableViewController.conditionsData[4] forKey:@"flightRules"];
        [object setValue:self->conditionsTableViewController.conditionsData[5] forKey:@"relativeHumidity"];
        [object setValue:self->conditionsTableViewController.conditionsData[6] forKey:@"cloudCoverage"];
        [object setValue:self->conditionsTableViewController.conditionsData[7] forKey:@"altitude"];
        [object setValue:self->conditionsTableViewController.conditionsData[8] forKey:@"visibility"];
        [object setValue:self->conditionsTableViewController.conditionsData[9] forKey:@"prevailingVision"];
        [object setValue:self->conditionsTableViewController.conditionsData[10] forKey:@"windSpeed"];
        [object setValue:self->conditionsTableViewController.conditionsData[11] forKey:@"variableWind"];
        [object setValue:self->conditionsTableViewController.conditionsData[12] forKey:@"name"];
        [object setValue:self->conditionsTableViewController.conditionsData[13] forKey:@"tempC"];
        
        NSError *error = nil;
        if ([context save:&error] == NO) {
            NSAssert(NO, @"Error saving context: %@\n%@", [error localizedDescription], [error userInfo]);
        } else {
            [self->favoritesTableViewController.dataArray addObject:object];
        }
    });
}

-(void) createForecastDictionary: (NSDictionary*)forecastDictionary {
    NSArrayx *conditions = [forecastDictionary valueForKey:@"conditions"];
    NSArray *items = [conditions valueForKey:@"conditions"];
    NSDictionary *dict = conditions[0];
}

@end
