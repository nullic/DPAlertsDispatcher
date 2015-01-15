//
//  NSError+DPCommons.m
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import "NSError+DPCommons.h"
#import "DPAlertsDispatcher.h"

NSString * const kDPErrorTitleKey = @"DPErrorTitleKey";
NSString * const kDPCurrentApplicationErrorDomain = @"CurrentApplicationErrorDomain";

@implementation NSError (DPCommons)

+ (instancetype)applicationErrorWithDescription:(NSString *)description code:(NSInteger)code {
    return [self applicationErrorWithDescription:description title:nil code:code];
}

+ (instancetype)applicationErrorWithDescription:(NSString *)description title:(NSString *)title code:(NSInteger)code {
    NSMutableDictionary *userInfo = [NSMutableDictionary dictionary];
    [userInfo setValue:description forKey:NSLocalizedDescriptionKey];
    [userInfo setValue:title forKey:kDPErrorTitleKey];
    return [self errorWithDomain:kDPCurrentApplicationErrorDomain code:code userInfo:userInfo];
}

- (instancetype)log {
    NSLog(@"Unresolved error: %@", self);
    return self;
}

- (instancetype)show {
    [[DPAlertsDispatcher defaultDispatcher] dispatchError:self];
    return self;
}

- (void)fail {
    NSLog(@"Unresolved error (critical error): %@", self);
    abort();
}

@end
