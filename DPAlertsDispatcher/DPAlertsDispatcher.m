//
//  DPAlertsDispatcher.m
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import "DPAlertsDispatcher.h"

dispatch_queue_t _dp_message_dispatcher_q = NULL;

static NSString * const kTitleKey = @"title";
static NSString * const kBodyKey = @"message";
static NSString * const kErrorKey = @"error";
static NSString * const kCancelTitleKey = @"cancel";
static NSString * const kDismissTitleKey = @"dismiss";
static NSString * const kCompletionKey = @"completion";

@interface DPAlertsDispatcher () <UIAlertViewDelegate>
@property (nonatomic, strong) NSMutableArray *messages;
@property (nonatomic, strong) UIAlertView *alertView;
@end

@implementation DPAlertsDispatcher

+ (void)load {
    @autoreleasepool {
        _dp_message_dispatcher_q = dispatch_queue_create("ind.dmitriy-petrusevich.alerts.dispatcher", DISPATCH_QUEUE_SERIAL);
    }
}

+ (instancetype)defaultDispatcher {
    static id _sharedInstance = nil;
    static dispatch_once_t onceToken = 0;

    dispatch_once(&onceToken, ^{
        _sharedInstance = [[self alloc] init];
    });

    return _sharedInstance;
}

- (NSMutableArray *)messages {
    if (!_messages) {
        _messages = [[NSMutableArray alloc] init];
    }
    return _messages;
}

- (NSString *)defaultErrorTitle {
    if (!_defaultErrorTitle) {
        _defaultErrorTitle = @"Error";
    }
    return _defaultErrorTitle;
}

- (NSString *)defaultDismissTitle {
    if (!_defaultDismissTitle) {
        _defaultDismissTitle = @"Dismiss";
    }
    return _defaultDismissTitle;
}

- (NSString *)defaultCancelTitle {
    if (!_defaultCancelTitle) {
        _defaultCancelTitle = @"Cancel";
    }
    return _defaultCancelTitle;
}

#pragma mark - Dispatch

- (void)dispatchError:(NSError *)error {
    [self dispatchError:error groupSame:YES canCancel:NO dismissButtonTitle:nil completion:nil];
}

- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel dismissButtonTitle:(NSString *)dismissButtonTitle completion:(dp_dispatcher_completion_block_t)completion
{
    if (error == nil) return;

    dispatch_async(_dp_message_dispatcher_q, ^{
        dispatch_block_t addBlock = ^{
            NSDictionary *userInfo = error.userInfo;

            NSMutableDictionary *messageDict = [NSMutableDictionary dictionaryWithObject:error forKey:kErrorKey];

            [messageDict setValue:userInfo[kDPErrorTitleKey] ? userInfo[kDPErrorTitleKey] : self.defaultErrorTitle forKey:kTitleKey];
            [messageDict setValue:error.localizedDescription forKey:kBodyKey];
            [messageDict setValue:(dismissButtonTitle ? dismissButtonTitle : self.defaultDismissTitle) forKey:kDismissTitleKey];
            [messageDict setValue:(canCancel ? self.defaultCancelTitle : nil) forKey:kCancelTitleKey];
            [messageDict setValue:[completion copy] forKey:kCompletionKey];

            [self.messages addObject:messageDict];
            [self showNextMessage];
        };

        if (groupSame == YES) {
            NSPredicate *predicate = [NSPredicate predicateWithFormat:@"%K.domain ==[cd] %@ AND %K.code == %d", kErrorKey, error.domain, kErrorKey, error.code];

            NSArray *filtArray = [self.messages filteredArrayUsingPredicate:predicate];
            if (filtArray.count == 0) {
                addBlock();
            }
        }
        else {
            addBlock();
        }
    });
}

- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel dismissButtonTitle:(NSString *)dismissButtonTitle completion:(dp_dispatcher_completion_block_t)completion
{
    if (message.length || title.length) {
        NSMutableDictionary *messageDict = [NSMutableDictionary dictionary];

        [messageDict setValue:title forKey:kTitleKey];
        [messageDict setValue:message forKey:kBodyKey];
        [messageDict setValue:[completion copy] forKey:kCompletionKey];
        [messageDict setValue:(dismissButtonTitle ? dismissButtonTitle : self.defaultDismissTitle) forKey:kDismissTitleKey];
        [messageDict setValue:(canCancel ? self.defaultCancelTitle : nil) forKey:kCancelTitleKey];

        dispatch_async(_dp_message_dispatcher_q, ^{
            [self.messages addObject:messageDict];
            [self showNextMessage];
        });
    }
}

#pragma mark - Show & Alert delegate

- (void)showNextMessage {
    if (self.messages.count == 0) return;
    NSDictionary *messageDict = self.messages[0];

    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.alertView != nil) return;

        self.alertView = [[UIAlertView alloc] initWithTitle:messageDict[kTitleKey] message:messageDict[kBodyKey] delegate:self cancelButtonTitle:messageDict[kCancelTitleKey] otherButtonTitles:messageDict[kDismissTitleKey], nil];
        [self.alertView show];
    });
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    if (alertView == self.alertView) {
        BOOL canceled = (buttonIndex == alertView.cancelButtonIndex);

        dispatch_async(_dp_message_dispatcher_q, ^{
            dp_dispatcher_completion_block_t completion = self.messages[0][kCompletionKey];
            [self.messages removeObjectAtIndex:0];
            if (completion) {
                dispatch_async(dispatch_get_main_queue(), ^{completion(canceled);});
            }

            self.alertView = nil;
            [self showNextMessage];
        });
    }
}

@end
