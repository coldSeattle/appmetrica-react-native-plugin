// Example AppDelegate.mm for integrating AppMetrica C++ modules
// This file shows how to register AppMetrica modules in an iOS app

#import <React/RCTBundleURLProvider.h>
#import <RCTAppDelegate+Protected.h>
#import "NativeAppMetrica.h"
#import "NativeAppMetricaReporter.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  self.moduleName = @"YourAppName";
  // You can add your custom initial props in the dictionary below.
  // They will be passed down to the ViewController used by React Native.
  self.initialProps = @{};

  return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

- (NSURL *)sourceURLForBridge:(RCTBridge *)bridge
{
#if DEBUG
  return [[RCTBundleURLProvider sharedSettings] jsBundleURLForBundleRoot:@"index"];
#else
  return [[NSBundle mainBundle] URLForResource:@"main" withExtension:@"jsbundle"];
#endif
}

/// This method controls whether the `concurrentRoot`feature of React18 is turned on or off.
///
/// @see: https://reactjs.org/blog/2022/03/29/react-v18.html
/// @note: This requires to be rendering on Fabric (i.e. on the New Architecture).
/// @return: `true` if the `concurrentRoot` feature is enabled. Otherwise, it returns `false`.
- (BOOL)concurrentRootEnabled
{
  return true;
}

// Register AppMetrica C++ modules
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:(const std::string &)name
                                                      jsInvoker:(std::shared_ptr<facebook::react::CallInvoker>)jsInvoker
{
  if (name == "NativeAppMetrica") {
    return std::make_shared<facebook::react::NativeAppMetrica>(jsInvoker);
  }
  
  if (name == "NativeAppMetricaReporter") {
    return std::make_shared<facebook::react::NativeAppMetricaReporter>(jsInvoker);
  }

  return [super getTurboModule:name jsInvoker:jsInvoker];
}

@end
