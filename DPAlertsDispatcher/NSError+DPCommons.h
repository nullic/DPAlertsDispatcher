//
//  NSError+DPCommons.h
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString * const kDPErrorTitleKey;

@interface NSError (DPCommons)
+ (instancetype)applicationErrorWithDescription:(NSString *)description code:(NSInteger)code;
+ (instancetype)applicationErrorWithDescription:(NSString *)description title:(NSString *)title code:(NSInteger)code;
@end
