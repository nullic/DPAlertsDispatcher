//
//  NSError+DPCommons.h
//  DPCommons
//
//  Created by dmitriy.petrusevich on 20.10.14.
//  Copyright (c) 2014 Dmitriy Petrusevich. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 @constant kDPErrorTitleKey
 @brief Key that used to store custom error title. Used by DPAlertsDispatcher to fill DPAlertInfo object.
 */
extern NSString * const kDPErrorTitleKey;

/**
 @constant kDPCurrentApplicationErrorDomain
 @brief Error domain used to create errors via +[applicationErrorWithDescription:] methods.
 */
extern NSString * const kDPCurrentApplicationErrorDomain;


@interface NSError (DPCommons)

/**
 @brief Create new error with kDPCurrentApplicationErrorDomain domain
 @param description     Localized error description.
 @param code            Error code.
 
 @return Result of invoking +[applicationErrorWithDescription:description title:nil code:code]
 */
+ (instancetype)applicationErrorWithDescription:(NSString *)description code:(NSInteger)code;

/**
 @brief Create new error with kDPCurrentApplicationErrorDomain domain
 @param description     Localized error description.
 @param title           Localized error title.
 @param code            Error code.
 
 @return New error instnace filled with localized values
 */
+ (instancetype)applicationErrorWithDescription:(NSString *)description title:(NSString *)title code:(NSInteger)code;

/**
 @brief Print error in console.
 @return <font color="AA0D91">self</font>
 */
- (instancetype)log;

/**
 @brief Dispatch error by invoking [[DPAlertsDispatcher defaultDispatcher] dispatchError:self]
 @return <font color="AA0D91">self</font>
 */
- (instancetype)show;

/**
 @brief Print error in console and call <font color="red">abort()</font>
 */
- (void)fail;
@end
