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

/**
 @class DPAlertInfo
 @brief Contains all information that will reqire to show an alert
*/
@interface DPAlertInfo : NSObject

/**
 @property title
 @brief Contains title of alert
 */
@property (nonatomic, copy) NSString *title;

/**
 @property message
 @brief Contains message that should be shown to user.
 */
@property (nonatomic, copy) NSString *message;

/**
 @property cancelButtonTitle
 @brief Contains title of cancel button or <font color="AA0D91">nil</font> if alert cannot be canceled.
 */
@property (nonatomic, copy) NSString *cancelButtonTitle;

/**
 @property dismissButtonTitle
 @brief Contains title of dismiss button.
 */
@property (nonatomic, copy) NSString *dismissButtonTitle;

/**
 @property dismissButtonTitle
 @brief Contains completion block.
 @discussion Block will be called by DPAlertsDispatcher instance, after alert was shown.
 */
@property (nonatomic, copy) dp_dispatcher_completion_block_t completion;

/**
 @property userInfo
 @brief Contains userInfo object that was passed to 'dispatch' methods.
 */
@property (nonatomic, strong) id userInfo;

/**
 @property userInfo
 @brief Contains error object that was passed to 'dispatch' methods.
 */
@property (nonatomic, strong) NSError *error;
@end


@protocol DPAlertsControllerProtocol <NSObject>
/**
 @brief Called when DPAlertsDispatcher should show alert.
 @param alertInfo   Contains all information, that was passed to 'dispatch' method.
 @param completion  Block that must be called after alert was shown.<br/><font color="AA0D91">YES</font> - if user press cancel button,<br/><font color="AA0D91">NO</font> - otherwise.

 @discussion 'completion' not equal alertInfo.completion.
 */
- (void)showAlertWithAlertInfo:(DPAlertInfo *)alertInfo completion:(dp_dispatcher_completion_block_t)completion;
@end


/**
 @class DPAlertsDispatcher
 @brief
 */
@interface DPAlertsDispatcher : NSObject <DPAlertsControllerProtocol>

/**
 @property alertsController
 @brief Used for customization of alerts. If eqult to <font color="#AA0D91">nil</font>, use <font color="#AA0D91">self</font> as 'alertsController'.
 */
@property (nonatomic, strong) id<DPAlertsControllerProtocol> alertsController;

/**
 @property defaultErrorTitle
 @brief Contains default title for 'error' alerts. Default value is @"Error".
 @discussion Cannot be <font color="#AA0D91">nil</font>.
 */
@property (nonatomic, copy) NSString *defaultErrorTitle;

/**
 @property defaultDismissTitle
 @brief Contains default title for dismiss button. Default value is @"Dismiss".
 @discussion Cannot be <font color="#AA0D91">nil</font>.
 */
@property (nonatomic, copy) NSString *defaultDismissTitle;

/**
 @property defaultCancelTitle
 @brief Contains default title for calcel button. Default value is @"Cancel".
 @discussion Cannot be <font color="#AA0D91">nil</font>.
 */
@property (nonatomic, copy) NSString *defaultCancelTitle;

/**
 @brief Returns shared DPAlertsDispatcher object
 @return Returns instance used for dispatch alerts.
 */
+ (instancetype)defaultDispatcher;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue from NSError object
 @param error   Error that should be shown

 @discussion Equal to call [<font color="AA0D91">self</font> dispatchError:error groupSame:YES canCancel:NO cancelButtonTitle:nil dismissButtonTitle:nil userInfo:nil completion:nil]
 */
- (void)dispatchError:(NSError *)error;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue from NSError object
 @param error               Error that should be shown
 @param groupSame           Skip alert if error with same domain and code was already dispatched.
 @param canCancel           Mark that alert should contains 'cancel' button. If value is <font color="AA0D91">NO</font>, 'cancelButtonTitle' will be ignored
 @param cancelButtonTitle   Title of 'cancel' button. If value is <font color="AA0D91">nil</font> 'defaultCancelTitle' will be used to create 'alertInfo'
 @param dismissButtonTitle  Title of 'dismiss' button. If value is <font color="AA0D91">nil</font> 'defaultDismissTitle' will be used to create 'alertInfo'
 @param userInfo            'userInfo' object that will be associated with 'alertInfo' object.
 @param completion          Block that will be invoked after alert was shown.
 
 @discussion Methos, in additional, set 'error' property in 'alertInfo' object; 'title' will be filled from userInfo of 'error' object (see kDPErrorTitleKey) or with 'defaultErrorTitle'.
 
 @discussion Invoke [<font color="AA0D91">self</font> dispatchAlertInfo:alertInfo] in the end.
 */
- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue with specific message
 @param message             Message that will be shown.
 @param title               Alert title.
 @param dismissButtonTitle  Title of 'dismiss' button. If value is <font color="AA0D91">nil</font> 'defaultDismissTitle' will be used to create 'alertInfo'.
 
 @discussion Equal to call [<font color="AA0D91">self</font> dispatchMessage:message withTitle:title canCancel:NO cancelButtonTitle:nil dismissButtonTitle:dismissButtonTitle userInfo:nil completion:nil]
 */
- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title dismissButtonTitle:(NSString *)dismissButtonTitle;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue with specific message
 @param message             Message that will be shown.
 @param title               Alert title.
 @param canCancel           Mark that alert should contains 'cancel' button. If value is <font color="AA0D91">NO</font>, 'cancelButtonTitle' will be ignored.
 @param cancelButtonTitle   Title of 'cancel' button. If value is <font color="AA0D91">nil</font> 'defaultCancelTitle' will be used to create 'alertInfo'.
 @param dismissButtonTitle  Title of 'dismiss' button. If value is <font color="AA0D91">nil</font> 'defaultDismissTitle' will be used to create 'alertInfo'.
 @param userInfo            'userInfo' object that will be associated with 'alertInfo' object.
 @param completion          Block that will be invoked after alert was shown.
 
 @discussion Invoke [<font color="AA0D91">self</font> dispatchAlertInfo:alertInfo] in the end.
 */
- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle dismissButtonTitle:(NSString *)dismissButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion;

/**
 @brief Add DPAlertInfo into alerts queue
 @param alertInfo   Contains information, that will be used to show alert
 */
- (void)dispatchAlertInfo:(DPAlertInfo *)alertInfo;
@end

/**
 @brief Dispatch alert with specific message
 @param message     Message that will be shown.
 @param completion  Block that will be invoked after alert was shown.
 
 @discussion Equal to call [[DPAlertsDispatcher defaultDispatcher] dispatchMessage:message withTitle:@"" canCancel:NO cancelButtonTitle:nil dismissButtonTitle:nil userInfo:nil completion:completion]
 */
void dp_show_message(NSString *message, dp_dispatcher_completion_block_t completion);
