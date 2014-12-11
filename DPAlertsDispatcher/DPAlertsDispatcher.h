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


@interface DPAlertInfo : NSObject
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *message;
@property (nonatomic, copy) NSString *cancelButtonTitle;
@property (nonatomic, copy) NSString *dismissButtonTitle;
@property (nonatomic, copy) dp_dispatcher_completion_block_t completion;
@property (nonatomic, strong) id userInfo;
@property (nonatomic, strong) NSError *error;
@end


@protocol DPAlertsControllerProtocol <NSObject>
- (void)showAlertWithAlertInfo:(DPAlertInfo *)alertInfo completion:(dp_dispatcher_completion_block_t)completion;
@end


@interface DPAlertsDispatcher : NSObject <DPAlertsControllerProtocol>
@property (nonatomic, strong) id<DPAlertsControllerProtocol> alertsController;
@property (nonatomic, copy) NSString *defaultErrorTitle;
@property (nonatomic, copy) NSString *defaultDismissTitle;
@property (nonatomic, copy) NSString *defaultCancelTitle;

+ (instancetype)defaultDispatcher;

- (void)dispatchError:(NSError *)error;
- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle completion:(dp_dispatcher_completion_block_t)completion;

- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title dismissButtonTitle:(NSString *)dismissButtonTitle;
- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion;
@end


#define SHOW_MESSAGE(message) ([[DPAlertsDispatcher defaultDispatcher] dispatchMessage:message withTitle:@"" canCancel:NO dismissButtonTitle:nil completion:nil])

#define FAIL_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error); abort();}
#define LOG_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error);}
#define SHOW_ON_ERROR(error) if(error) {NSLog(@"Unresolved error: %@", error); [[DPAlertsDispatcher defaultDispatcher] dispatchError:error];}
