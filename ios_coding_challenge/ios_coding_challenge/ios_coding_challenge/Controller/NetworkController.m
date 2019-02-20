//
//  NetworkController.m
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/17/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import "NetworkController.h"

@implementation NetworkController

-(instancetype)init {
    self = [super init];
    
    return self;
}

- (void)getSearchResults: (NSString*)searchTerm completion:(void(^)(void))callback{
//    [dataTask cancel];
    
    NSURL *url = [NSURL URLWithString:@"https://qa.foreflight.com/weather/report/"];
    url = [url URLByAppendingPathComponent:searchTerm];
    NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
    
    // Configure the session here.
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:configuration];
    
    [[session dataTaskWithURL:url
            completionHandler:^(NSData *data, NSURLResponse *response, NSError *error)
    {
        // The response object contains the metadata (HTTP headers, status code)
        // The data object contains the response body
        
        // The error object contains any client-side errors (e.g. connection
        // failures) and, in some cases, may report server-side errors.
        // In general, however, you should detect server-side errors by
        // checking the HTTP status code in the response object.
    }] resume];
}


@end
