//
//  NetworkController.h
//  ios_coding_challenge
//
//  Created by Charlie Barber on 2/17/19.
//  Copyright © 2019 Charlie Barber. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NetworkController : NSObject {
    NSURLSession *defaultSession;
    NSURLSessionDataTask *dataTask;
    NSURLComponents *urlComponents;
    NSURL *url;
    NSHTTPURLResponse *response;
}

- (void)getSearchResults: (NSString*)searchTerm completion:(void(^)(void))callback;

@end
