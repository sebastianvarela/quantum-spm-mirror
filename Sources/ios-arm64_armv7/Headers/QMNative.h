//
//  QMNative.h
//  QMNative
//
//  Copyright © 2017 Quantum Metric. All rights reserved.
//
//    See the accompanying ATTRIBUTIONS file for open source attributions
//


#import <Foundation/Foundation.h>
@import UIKit;


/**
 These are Event Flags, which allow you to specify special parameters for events you send using the SDK.
 For example, sending an event with a value of "12.34" and flag of `QMEventTypeConversion` will mark that event as a $12.34 conversion (or whatever currency your sub uses).
 Examples:
 ```swift
 QMNative.sendEvent(withID: 12, value: "19.95", flags: .conversion);
 ```
 Objective-C:
 ```objc
 [QMNative sendEventWithID:12 value:@"19.95" flags:QMEventTypeConversion];
 ```
 ### Event ID
 
 You can find the event id of the event you'd like to fire in your code by going to Settings > Events within your Quantum Metric subscription interface on the web. If you do not have any events setup, you can set one up there, and it'll create a new event id for you to use to send events.
 
 ## Eventing use cases
 
 Although Quantum can remotely configure a number of events, sending some events within your code will allow you to harness the full power of Quantum. Common use cases for sending events manually include:
 
 ### Tracking internal errors
 
 Almost every developer has some kind of comment in their code that looks like this:
 ```objc
 if (...) {
    //...
 } else {
    //this should never happen!
 }
 ```
 
 But somehow, the `//this should never happen!` part, happens! If this is occuring for you, and you want to see what the user did in order to get to that point, you can fire an event (and as the event value, maybe even pass in some debug information to give you some more clues).
 
 Let's take a look at what we could send:
 ```objc
 if (...) {
    //...
 } else {
    //this should never happen, but if it does, let's fire an event so we can see these sessions in Quantum
    [QMNative sendEventWithID:2 value:[NSString stringWithFormat:@"dictionary: %@ integer: %i, string: %@", myDictionary, myInteger, myString]];
 }
 ```
 
 This way, the value will contain your dictionary/integer/string or whatever else you want to pass through, to help debug further.
 
 ### Tracking occurences of things happening
 
 You may want to track some interactions, like how many search results they view before selecting one, or how many images they view before deciding to add a product to their cart. These types of examples may require writing a little code to keep track of the number of interactions that happen, and then sending off that value.
 
 ```objc
 [QMNative sendEventWithID:3 value:[NSString stringWithFormat:@"%i", numberOfImagesUserViewed]];
 ```

 ### Time to action
 
 Another common metric is how long it takes users to complete a task. If you record the times users do things, such as view cart, and then find the time difference when the checkout completes, you can record (in seconds), how long that took, if your goal was to optimize for faster/easier checkout times.
 
 ```objc
 [QMNative sendEventWithID:4 value:[NSString stringWithFormat:@"%i", numberOfSecondsBetweenUserActions]];
 ```

 These are just some ideas to get you started. Quantum can measure whatever you pass it, so you can keep an eye on various metrics, and setup alerts if something goes higher or lower than expected. If you have any questions, please contact us so we can help!
 
 */
typedef NS_OPTIONS(NSUInteger, QMEventTypeFlag) {
    /// This flag specifies the event was a conversion, i.e. the user completed checkout or some other goal within your app
    QMEventTypeConversion = (1 << 0),
    /// This flag specifies the event was a login action, the user succesfully logged in. The value here is the username that is used to track this user across platforms in Quantum. Say the user logged in and your app uses email addresses as the login type, if their address was user@example.com, and on web they also login using user@example.com, Quantum will tie these sessions to the same user. The login value does not have to be an email address, it could be a phone number, user id, rewards number, screen name, etc. The key component to tie users together is that the string must be the same across platforms.
    QMEventTypeLogin = (1 << 1),
    /// This flag specifies the user's email address. It is not used to identify the same users across platforms or accounts, for example, multiple different users could have the same email address and it would not tie them to the same user, for that you would want the `QMEventTypeLogin` flag. However, this will save the email address for that user, and the email address can be used to search and find that user.
    QMEventTypeEmail = (1 << 2),
    /// Saves the first name for this user.
    QMEventTypeFirstName = (1 << 3),
    /// Saves the last name for this user.
    QMEventTypeLastName = (1 << 4),
    /// Updates the cart value for the user. Whenever the user adds or removes something to their cart, you can send an event where the value is the new cart value. This can be sent multiple times to show the progress of the users activity throughout the session. When a conversion is sent, you do not have to reset the cart value to zero.
    QMEventTypeCartValue = (1 << 6),
    /// This flag specifies the event is part of A/B testing.
    QMEventTypeABN = (1 << 7),
    /// This flag will cause the value to be encrypted, so that when viewing the replay, you must have the Private Key in order to decrypt it and see the original value.
    QMEventTypeEncrypted = (1 << 8),
    /// This flag allows you to specify what type of application is currently running. Quantum sends this by default, but you can override it if you wish.
    QMEventTypeApplicationType = (1 << 10),
    /// This flag specifies that the sent event value contains the app's version. Quantum sends this automatically without requiring you to do so.
    QMEventTypeApplicationVersion = (1 << 11)
};

/**
 * The entry point of the Quantum Metric Native SDK.
 *
 * To get started, import the header file, and call `[QMNative initializeWithSubscription:@"yoursubscription" uid:@"youruid"];`. This method should always be called first. If you're not sure what your subscription name or UID are, contact us.
 *
 * After setup, you can access the QMNative object with `-sharedInstance`, and call other API methods on it. For example: `[[QMNative sharedInstance] sendEventWithID:4 value:@"test value"];`
 */

@interface QMNative : NSObject

/// Your subscription name (i.e., yourcompany.quantummetric.com). Note that you should not modify this value after you've initialized QMNative.
@property(nonatomic, strong) NSString *subscription;
/// Your UID
@property(nonatomic, strong) NSString *uid;

/**
 Access the QMNative singleton object.
 */
+ (id)sharedInstance;

/**
 Call this in the beginning of your `application:didFinishLaunchingWithOptions:` method in your application delegate to initialize Quantum Metric.
 
 @param subscription The subscription name, visible in your Quantum Metric Dashboard.
 @param uid The UID assigned to you in your Quantum Metric Dashboard.
 */
+ (void)initializeWithSubscription:(NSString*)subscription uid:(NSString*)uid;

/**
 For testing your iOS configuration settings before going live, set this to true just after calling `initializeWithSubscription`.
 
 @param testing If this is true, the test configuration will be used. False will use the production configuration.
 */
+(void)enableTestConfig:(BOOL)testing;
/**
 This method is being deprecated, please use the method above.
 */
-(void)enableTestConfig:(BOOL)testing DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.enableTestConfig() or [QMNative enableTestConfig:] instead.");

/**
 Send an event
 
 @param eventID The ID of the event to be logged to the server
 @param value The value of the event to be logged to the server
 
 This is the same as calling the below method with flags:0
 */
+(void)sendEventWithID:(NSInteger)eventID value:(NSString*)value;
/**
 This method is being deprecated, please use the method above.
 */
-(void)sendEventWithID:(NSInteger)eventID value:(NSString*)value DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.sendEvent(withID:, value:) or [QMNative sendEventWithID:value:] instead.");

/**
 Send an event, with a flag
 
 @param eventID The ID of the event to be logged to the server
 @param value The value of the event to be logged to the server
 @param flags The flags for the event to be logged. Only used for special occasions.
 */
+(void)sendEventWithID:(NSInteger)eventID value:(NSString*)value flags:(QMEventTypeFlag)flags;
/**
 This method is being deprecated, please use the method above.
 */
-(void)sendEventWithID:(NSInteger)eventID value:(NSString*)value flags:(QMEventTypeFlag)flags DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.sendEvent(withID:, value:, flags:) or [QMNative sendEventWithID:value:flags:] instead.");

/**
 Send an error
 
 @param errorID The ID of the error to be logged to the server
 @param value The value of the error to be logged to the server
 */
+(void)sendErrorWithID:(NSInteger)errorID value:(NSString*)value;
/**
 This method is being deprecated, please use the method above.
 */
-(void)sendErrorWithID:(NSInteger)errorID value:(NSString*)value DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.sendError(withID: , value: ) or [QMNative sendErrorWithID:value:] instead.");

/**
 Ties passed in user information to this session.
 
 @param login Login for the user. This can be the user's email address, username, a user id, or whatever you use as your generic user identifier.
 */
+(void)setUserLogin:(NSString*)login;

/**
 Ties passed in user information to this session.
 
 @param firstName First Name of the user
 */
+(void)setUserFirstName:(NSString*)firstName;
/**
 This method is being deprecated, please use the method above.
 */
-(void)setUserFirstName:(NSString*)firstName DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.setUserFirstName() or [QMNative setUserFirstName:] instead.");

/**
 Ties passed in user information to this session.
 
 @param lastName Last Name of the user
 */
+(void)setUserLastName:(NSString*)lastName;
/**
 This method is being deprecated, please use the method above.
 */
-(void)setUserLastName:(NSString*)lastName DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.setUserLastName() or [QMNative setUserLastName:] instead.");

/**
 Ties passed in user information to this session.
 
 @param email Email address of the user
 */
+(void)setUserEmail:(NSString*)email;
/**
 This method is being deprecated, please use the method above.
 */
-(void)setUserEmail:(NSString*)email DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.setUserEmail() or [QMNative setUserEmail:] instead.");

/**
 Updates the cart value for this session
 
 @param newCartValueCents The new cart value, in cents, as an NSInteger. A value of 10000 would be $100.00, or 100.00 of whatever currency is associated with that session.
 */
+(void)updateCartValue:(NSInteger)newCartValueCents;
/**
 This method is being deprecated, please use the method above.
 */
-(void)updateCartValue:(NSInteger)newCartValueCents DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.updateCartValue() or [QMNative updateCartValue:] instead.");


/**
 Indicates that a conversion happened, with the given cart value.
 
 @param convertedCartValueCents The converted cart value, in cents, as an NSInteger. Same as updateCartValue:, but call this after a conversion happened.
 */
+(void)convertedCartValue:(NSInteger)convertedCartValueCents;
/**
 This method is being deprecated, please use the method above.
 */
-(void)convertedCartValue:(NSInteger)convertedCartValueCents DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.convertedCartValue() or [QMNative convertedCartValue:] instead.");

/**
 Enables encrption with the given public key. You should call this right after initializeWithSubscription:uid:
 
 @param encryptionKey Public key for encryption.
 */
+(void)enableEncryptionWithKey:(NSString*)encryptionKey;
/**
 This method is being deprecated, please use the method above.
 */
-(void)enableEncryptionWithKey:(NSString*)encryptionKey DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.enableEncryption(withKey: ) or [QMNative enableEncryptionWithKey:] instead.");

/**
 Marks a view as private, we don't want the personal data in that view, or any subviews, to leave the device. Note this is a static method, so call [QMNative maskView:yourCoolView];
 
 @param view The view to be masked. It's important to note that this method should be called before the view is added to the visible view hierarchy, otherwise the information in that view will have already been captured.
 */
+(void)maskView:(UIView*)view;

/**
 Marks a view as private, similar to maskView, except this will block all text in it's view and subviews. Note this is a static method, so call [QMNative maskTextInView:yourCoolView];
 
 @param view The view to be masked. It's important to note that this method should be called before the view is added to the visible view hierarchy, otherwise the information in that view will have already been captured.
 */
+(void)maskTextInView:(UIView*)view;
/**
 Marks all views of a certain class as private, we don't want the personal data in that view, or any subviews, to leave the device. Similar to above. An example call might look like [QMNative maskViewClass:[QMAwesomeView class]];
 
 @param viewClass The view class to be masked. It's important to note that this method should be called before any views of this class are added to the visible view hierarchy. A suitable place to call this might be right afer the QMNative init method in your App Delegate.
 */
+(void)maskViewClass:(Class)viewClass;

/**
 Marks a view as needing encryption. Note this is a static method, so call [QMNative encryptView:yourCoolView];
 
 @param view The view to be encrypted. It's important to note that this method should be called before the view is added to the visible view hierarchy, otherwise the information in that view will have already been captured.
 */
+(void)encryptView:(UIView*)view;

/**
 Marks a view as not needing to be masked. Note this is a static method, so call [QMNative unmaskView:yourCoolView];
 
 @param view The view to be unmasked. If this view has yet to be masked, this will do nothing. If the view has already been masked, it and its subviews will no longer be masked. This is primarily useful when your subscription is using the "mask everything" configuration, as this allows you to target this view as safe.
 */
+(void)unmaskView:(UIView*)view;

/**
 Returns this version of the QMNative library
 
 @return Library version in the format of major.minor.patch, like @"1.4.2"
 */
+(NSString*)libraryVersionString;
/**
 This method is being deprecated, please use the method above.
 */
-(NSString*)libraryVersionString DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.libraryVersionString() or [QMNative libraryVersionString:] instead.");


/**
 Pauses Quantum Metric session recording, if it's already been started. These methods will be deprecated in favor of better names below.
 */
+(void)stopQM;
/**
 This method is being deprecated, please use the method above.
 */
-(void)stopQM DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.stopQM() or [QMNative stopQM] instead.");

/**
Pauses Quantum Metric session recording, if it's already been started. This is useful if you want to not capture a certain part of your app, and want to be able to start capture again on the same session.
*/
+(void)pauseSession;
/**
 This method is being deprecated, please use the method above.
 */
-(void)pauseSession DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.pauseSession() or [QMNative pauseSession] instead.");

/**
Stops capture and ends the session. To start capture again, you must start a new session using -newSession:
*/
+(void)endSession;
/**
 This method is being deprecated, please use the method above.
 */
-(void)endSession DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.endSession() or [QMNative endSession] instead.");

/**
 Restarts Quantum Metric session recording, if it's been stopped, with the same session
 */
+(void)restartQM;
/**
 This method is being deprecated, please use the method above.
 */
-(void)restartQM DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.enableTestConfig() or [QMNative restartQM] instead.");

/**
 Ends the current session and starts a new session
 */
+(void)newSession:(BOOL)resetUser;
/**
 This method is being deprecated, please use the method above.
 */
-(void)newSession:(BOOL)resetUser DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.newSession() or [QMNative newSession:] instead.");

/**
 Opts this user out of Quantum Metric. Note that this opt-out is for this install of the app, if they delete and reinstall, they might have to opt-out again. If your app has some way of keeping track of user opt-out preferences across devices/installs, and you want to insure this setting persists across devices/installs, be sure to call this method if you detect a fresh install (also, you can always check the users opt-in/out status with the userOptStatus method below.
 */
+(void)optUserOut;
/**
 This method is being deprecated, please use the method above.
 */
-(void)optUserOut DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.optUserOut() or [QMNative optUserOut:] instead.");

/**
 Opts this user back into Quantum Metric. Note that you do not need to call this to enable QM in the first place, it's enabled by default. Only call this if a user was previously opted out. On the next app launch, QM will run it's magic.
 */
+(void)optUserBackIn;
/**
 This method is being deprecated, please use the method above.
 */
-(void)optUserBackIn DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.optUserBackIn() or [QMNative optUserBackIn:] instead.");

/**
 Returns the current status for user opt in/out. True means that QM is enabled for this user, false means not enabled (false means they opted out).
 */
+(BOOL)userOptStatus;
/**
 This method is being deprecated, please use the method above.
 */
-(BOOL)userOptStatus DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.userOptStatus() or [QMNative userOptStatus] instead.");

/**
 Enables you to pass a block that gets called with the QM session cookie and QM user string when a session starts. The callback block will be called on the main thread.
 */
+(void)gotSessionCookieCallback:(void (^)(NSString *sessionCookie, NSString *userString))callback;
/**
 This method is being deprecated, please use the method above.
 */
-(void)gotSessionCookieCallback:(void (^)(NSString *sessionCookie, NSString *userString))callback DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.gotSessionCookieCallback() or [QMNative gotSessionCookieCallback:] instead.");

/**
 By default, your app's name will be used as the browser name in Quantum Metric. If you'd like to customize this, to more easily keep track of variations of an app (i.e. add "production", "dev", "beta") you can do that here. This must be called directly after initializeWithSubscription:uid:
 */
+(void)setBrowserString:(NSString*)browserString;
/**
 This method is being deprecated, please use the method above.
 */
-(void)setBrowserString:(NSString*)browserString DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.setBrowserString() or [QMNative setBrowserString:] instead.");


/**
 By default, Quantum Metric sets itself as the default crash handler (if QM has started, based on your sampling settings). If you'd like to disable this, call this method right after `initializeWithSubscription`.
 */
+(void)disableCrashReporting;
/**
 This method is being deprecated, please use the method above.
 */
-(void)disableCrashReporting DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.disableCrashReporting() or [QMNative disableCrashReporting] instead.");

/**
 Enables you to receive a callback when the SDK detects a rageclick.
 */
+(void)gotPossibleFrustrationCallback:(void (^)(UIView *view))callback;
/**
 This method is being deprecated, please use the method above.
 */
-(void)gotPossibleFrustrationCallback:(void (^)(UIView *view))callback DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.gotPossibleFrustrationCallback() or [QMNative gotPossibleFrustrationCallback:] instead.");

/**
Enables SSL certificate pinning for requests to Quantum Metric's servers. Please contact us if you'd like to enable this.
 (If you do enable it, you'll need to deploy a new build of the library when we get new SSL certificates)
*/
+(void)enableSSLCertificatePinning;
/**
 This method is being deprecated, please use the method above.
 */
-(void)enableSSLCertificatePinning DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.enableSSLCertificatePinning() or [QMNative enableSSLCertificatePinning] instead.");

/**
By default, the SDK determines where "pages" are in your application, normally based on view controllers appearing.
 However, some apps, like those built with ReactNative, don't use view controllers, and determining when a new page
 occurs can be difficult. This method allows you to tell QM when to send a new page.
*/
+(void)sendNewPage;
/**
By default, the SDK determines where "pages" are in your application, normally based on view controllers appearing.
 However, some apps, like those built with ReactNative, don't use view controllers, and determining when a new page
 occurs can be difficult. This method allows you to tell QM when to send a new page.
 
 @param pageName Pass in the "name" of your page to tell Quantum what you'd like it to show in the replay
*/
+(void)sendNewPageNamed:(NSString*)pageName;

/**
 Returns the Quantum Metric replay URL of the current session.
 */
+(NSString *)getCurrentReplayURL;
/**
 Returns the Quantum Metric replay URL of the current session.
 */
-(NSString *)getCurrentReplayURL DEPRECATED_MSG_ATTRIBUTE("Quantum Metric is deprecating the use of instance methods, use QMNative.getCurrentReplayURL() or [QMNative getCurrentReplayURL] instead.");
@end
