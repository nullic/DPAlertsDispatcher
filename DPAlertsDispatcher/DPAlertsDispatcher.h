//
//  DPAlertsDispatcher.h
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NSError+DPCommons.h"

typedef void(^dp_dispatcher_completion_block_t)(BOOL canceled);

@interface DPAlertsDispatcher : NSObject
@property (nonatomic, copy) NSString *defaultErrorTitle;
@property (nonatomic, copy) NSString *defaultDismissTitle;
@property (nonatomic, copy) NSString *defaultCancelTitle;

+ (instancetype)defaultDispatcher;

- (void)dispatchError:(NSError *)error;
- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel dismissButtonTitle:(NSString *)dismissButtonTitle completion:(dp_dispatcher_completion_block_t)completion;

- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel dismissButtonTitle:(NSString *)dismissButtonTitle completion:(dp_dispatcher_completion_block_t)completion;
@end

#define SHOW_MESSAGE(message) ([[DPAlertsDispatcher defaultDispatcher] dispatchMessage:message withTitle:@"" canCancel:NO dismissButtonTitle:nil completion:nil])

#define FAIL_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error); abort();}
#define LOG_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error);}
#define SHOW_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error); [[DPAlertsDispatcher defaultDispatcher] dispatchError:error];}