//
//  DPAlertsDispatcher.m
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import "DPAlertsDispatcher.h"

dispatch_queue_t _dp_message_dispatcher_q = NULL;

@implementation DPAlertInfo
@end

@interface DPAlertsDispatcher () <UIAlertViewDelegate>
@property (nonatomic, strong) NSMutableArray *messages;
@property (nonatomic, getter=isAlertShown) BOOL alertShown;
@property (nonatomic, copy) dp_dispatcher_completion_block_t alertShownCompletion;
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
    [self dispatchError:error groupSame:YES canCancel:NO cancelButtonTitle:nil dismissButtonTitle:nil userInfo:nil completion:nil];
}

- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion
{
    if (error == nil) return;

    dispatch_async(_dp_message_dispatcher_q, ^{
        dispatch_block_t addBlock = ^{
            NSDictionary *userInfo = error.userInfo;

            DPAlertInfo *alertInfo = [DPAlertInfo new];
            alertInfo.error = error;
            alertInfo.userInfo = userInfo;

            NSString *body = error.localizedDescription;
            if (body == nil) {
                body = error.localizedRecoverySuggestion;
            }
            else if (error.localizedRecoverySuggestion) {
                body = [body stringByAppendingFormat:@"\n%@", error.localizedRecoverySuggestion];
            }

            alertInfo.title = userInfo[kDPErrorTitleKey] ? userInfo[kDPErrorTitleKey] : self.defaultErrorTitle;
            alertInfo.message = body;
            alertInfo.dismissButtonTitle = dismissButtonTitle ? dismissButtonTitle : self.defaultDismissTitle;
            alertInfo.cancelButtonTitle = canCancel ? (cancelButtonTitle ? cancelButtonTitle : self.defaultCancelTitle) : nil;
            alertInfo.completion = completion;

            [self _dispatchAlertInfo:alertInfo];
        };

        if (groupSame == YES) {
            NSPredicate *predicate = [NSPredicate predicateWithFormat:@"error.domain ==[cd] %@ AND error.code == %d", error.domain, error.code];

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

- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title dismissButtonTitle:(NSString *)dismissButtonTitle {
    [self dispatchMessage:message withTitle:title canCancel:NO cancelButtonTitle:nil dismissButtonTitle:dismissButtonTitle userInfo:nil completion:nil];
}

- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion
{
    if (message.length || title.length) {
        DPAlertInfo *alertInfo = [DPAlertInfo new];

        alertInfo.title = title;
        alertInfo.message = message;
        alertInfo.dismissButtonTitle = dismissButtonTitle ? dismissButtonTitle : self.defaultDismissTitle;
        alertInfo.cancelButtonTitle = canCancel ? (cancelButtonTitle ? cancelButtonTitle : self.defaultCancelTitle) : nil;
        alertInfo.completion = completion;
        alertInfo.userInfo = userInfo;

        [self _dispatchAlertInfo:alertInfo];
    }
}

- (void)dispatchAlertInfo:(DPAlertInfo *)alertInfo {
    dispatch_async(_dp_message_dispatcher_q, ^{
        [self _dispatchAlertInfo:alertInfo];
    });
}

- (void)_dispatchAlertInfo:(DPAlertInfo *)alertInfo {
    [self.messages addObject:alertInfo];
    [self showNextMessage];
}

#pragma mark - Show & Alert delegate

- (void)showNextMessage {
    if (self.messages.count == 0) return;
    DPAlertInfo *alertInfo = self.messages[0];

    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.isAlertShown == YES) return;

        dp_dispatcher_completion_block_t completion = ^(BOOL canceled) {
            dispatch_async(_dp_message_dispatcher_q, ^{
                [self.messages removeObject:alertInfo];

                if (alertInfo.completion) {
                    dispatch_async(dispatch_get_main_queue(), ^{alertInfo.completion(canceled);});
                }

                self.alertShown = NO;
                self.alertShownCompletion = nil;
                [self showNextMessage];
            });
        };

        id<DPAlertsControllerProtocol> alertsController = self.alertsController ? self.alertsController : self;
        [alertsController showAlertWithAlertInfo:alertInfo completion:completion];
        self.alertShown = YES;
    });
}

#pragma mark -

- (void)showAlertWithAlertInfo:(DPAlertInfo *)alertInfo completion:(dp_dispatcher_completion_block_t)completion {
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:alertInfo.title message:alertInfo.message delegate:self cancelButtonTitle:alertInfo.cancelButtonTitle otherButtonTitles:alertInfo.dismissButtonTitle, nil];
    [alertView show];
    self.alertShownCompletion = completion;
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    BOOL canceled = (buttonIndex == alertView.cancelButtonIndex);

    if (self.alertShownCompletion) {
        self.alertShownCompletion(canceled);
    }
}

@end

// MARK: - Functions

void dp_show_message(NSString *message, dp_dispatcher_completion_block_t completion) {
    [[DPAlertsDispatcher defaultDispatcher] dispatchMessage:message withTitle:@"" canCancel:NO cancelButtonTitle:nil dismissButtonTitle:nil userInfo:nil completion:completion];
}
