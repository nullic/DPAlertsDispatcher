//
//  NSError+DPCommons.m
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import "NSError+DPCommons.h"

NSString * const kDPErrorTitleKey = @"DPErrorTitleKey";

@implementation NSError (DPCommons)

+ (NSString *)applicationErrorDomain {
    return @"CurrentApplicationErrorDomain";
}

+ (instancetype)applicationErrorWithDescription:(NSString *)description code:(NSInteger)code {
    return [self applicationErrorWithDescription:description title:nil code:code];
}

+ (instancetype)applicationErrorWithDescription:(NSString *)description title:(NSString *)title code:(NSInteger)code {
    NSMutableDictionary *userInfo = [NSMutableDictionary dictionary];
    [userInfo setValue:description forKey:NSLocalizedDescriptionKey];
    [userInfo setValue:title forKey:kDPErrorTitleKey];
    return [self errorWithDomain:[self applicationErrorDomain] code:code userInfo:userInfo];
}

@end
