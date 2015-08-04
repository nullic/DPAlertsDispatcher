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
 @property actionButtonTitle
 @brief Contains title of action button.
 */
@property (nonatomic, copy) NSString *actionButtonTitle;

/**
 @property completion
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
 @property defaultActionTitle
 @brief Contains default title for action button. Default value is @"Dismiss".
 @discussion Cannot be <font color="#AA0D91">nil</font>.
 */
@property (nonatomic, copy) NSString *defaultActionTitle;

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

 @discussion Equal to call [<font color="AA0D91">self</font> dispatchError:error groupSame:YES canCancel:NO cancelButtonTitle:nil actionButtonTitle:nil userInfo:nil completion:nil]
 */
- (void)dispatchError:(NSError *)error;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue from NSError object
 @param error               Error that should be shown
 @param groupSame           Skip alert if error with same domain and code was already dispatched.
 @param canCancel           Mark that alert should contains 'cancel' button. If value is <font color="AA0D91">NO</font>, 'cancelButtonTitle' will be ignored
 @param cancelButtonTitle   Title of 'cancel' button. If value is <font color="AA0D91">nil</font> 'defaultCancelTitle' will be used to create 'alertInfo'
 @param actionButtonTitle   Title of 'action' button. If value is <font color="AA0D91">nil</font> 'defaultActionTitle' will be used to create 'alertInfo'
 @param userInfo            'userInfo' object that will be associated with 'alertInfo' object.
 @param completion          Block that will be invoked after alert was shown.
 
 @discussion Methos, in additional, set 'error' property in 'alertInfo' object; 'title' will be filled from userInfo of 'error' object (see kDPErrorTitleKey) or with 'defaultErrorTitle'.
 */
- (void)dispatchError:(NSError *)error groupSame:(BOOL)groupSame canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle actionButtonTitle:(NSString *)actionButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue with specific message
 @param message             Message that will be shown.
 @param title               Alert title.
 @param actionButtonTitle   Title of 'action' button. If value is <font color="AA0D91">nil</font> 'defaultActionTitle' will be used to create 'alertInfo'.
 
 @discussion Equal to call [<font color="AA0D91">self</font> dispatchMessage:message withTitle:title canCancel:NO cancelButtonTitle:nil actionButtonTitle:actionButtonTitle userInfo:nil completion:nil]
 */
- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title actionButtonTitle:(NSString *)actionButtonTitle;

/**
 @brief Create DPAlertInfo instance, and add it into alerts queue with specific message
 @param message             Message that will be shown.
 @param title               Alert title.
 @param canCancel           Mark that alert should contains 'cancel' button. If value is <font color="AA0D91">NO</font>, 'cancelButtonTitle' will be ignored.
 @param cancelButtonTitle   Title of 'cancel' button. If value is <font color="AA0D91">nil</font> 'defaultCancelTitle' will be used to create 'alertInfo'.
 @param actionButtonTitle   Title of 'action' button. If value is <font color="AA0D91">nil</font> 'defaultActionTitle' will be used to create 'alertInfo'.
 @param userInfo            'userInfo' object that will be associated with 'alertInfo' object.
 @param completion          Block that will be invoked after alert was shown.
 */
- (void)dispatchMessage:(NSString *)message withTitle:(NSString *)title canCancel:(BOOL)canCancel cancelButtonTitle:(NSString *)cancelButtonTitle actionButtonTitle:(NSString *)actionButtonTitle userInfo:(id)userInfo completion:(dp_dispatcher_completion_block_t)completion;

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
 
 @discussion Equal to call [[DPAlertsDispatcher defaultDispatcher] dispatchMessage:message withTitle:@"" canCancel:NO cancelButtonTitle:nil actionButtonTitle:nil userInfo:nil completion:completion]
 */
void dp_show_message(NSString *message, dp_dispatcher_completion_block_t completion);

/**
 @brief Dispatch alert with specific message
 @param error       Error that should be shown
 @param completion  Block that will be invoked after alert was shown.

 @discussion Equal to call [[DPAlertsDispatcher defaultDispatcher] dispatchError:error groupSame:NO canCancel:NO cancelButtonTitle:nil actionButtonTitle:nil userInfo:nil completion:completion]
 */
void dp_show_error(NSError *error, dp_dispatcher_completion_block_t completion);