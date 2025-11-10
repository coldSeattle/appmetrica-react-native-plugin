# AppMetrica React Native - New Architecture Integration

This document explains how to integrate AppMetrica React Native plugin with the new architecture (Fabric + TurboModules).

## Prerequisites

- React Native 0.74+
- New Architecture enabled (`newArchEnabled=true`)
- Android NDK 21.4.7075529+
- CMake 3.13+

## Android Integration

### 1. Update your app's `build.gradle`

Add the following to your app's `android/app/build.gradle`:

```gradle
android {
    // ... existing configuration ...

    externalNativeBuild {
        cmake {
            path "../../node_modules/@appmetrica/react-native-analytics/android/app/src/main/jni/CMakeLists.txt"
            version "3.13.0"
        }
    }

    ndkVersion "21.4.7075529"
}

// Add this to ensure generated files are available
android {
    sourceSets {
        main {
            jniLibs.srcDirs = ['../../node_modules/@appmetrica/react-native-analytics/build/generated/android/app/build/generated/source/codegen/jni']
        }
    }
}
```

### 2. Update your app's `MainApplication.java`

Add the following to your `MainApplication.java`:

```java
import com.facebook.react.ReactApplication;
import com.facebook.react.ReactNativeHost;
import com.facebook.react.ReactPackage;
import com.facebook.react.shell.MainReactPackage;
import com.facebook.soloader.SoLoader;

import java.util.Arrays;
import java.util.List;

public class MainApplication extends Application implements ReactApplication {

    private final ReactNativeHost mReactNativeHost = new ReactNativeHost(this) {
        @Override
        public boolean getUseDeveloperSupport() {
            return BuildConfig.DEBUG;
        }

        @Override
        protected List<ReactPackage> getPackages() {
            return Arrays.<ReactPackage>asList(
                new MainReactPackage(),
                new AppMetricaPackage() // Add this line
            );
        }

        @Override
        protected String getJSMainModuleName() {
            return "index";
        }
    };

    @Override
    public ReactNativeHost getReactNativeHost() {
        return mReactNativeHost;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        SoLoader.init(this, /* native exopackage */ false);
    }
}
```

## iOS Integration

### 1. Update your app's `AppDelegate.mm`

Add the following to your `AppDelegate.mm`:

```objc
#import <React/RCTBundleURLProvider.h>
#import "NativeAppMetrica.h"
#import "NativeAppMetricaReporter.h"

@implementation AppDelegate

// ... existing methods ...

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
```

### 2. Run pod install

```bash
cd ios
pod install
```

## JavaScript Usage

### 1. Import the modules

```typescript
import NativeAppMetrica from './specs/NativeAppMetrica';
import NativeAppMetricaReporter from './specs/NativeAppMetricaReporter';
```

### 2. Use the modules

```typescript
// Initialize AppMetrica
NativeAppMetrica.activate({
  apiKey: 'YOUR_API_KEY',
  appVersion: '1.0.0',
  crashReporting: true,
  locationTracking: true,
});

// Report events
NativeAppMetrica.reportEvent('test_event', { key: 'value' });

// Use Reporter
const reporter = NativeAppMetricaReporter;
reporter.reportEvent('reporter_api_key', 'reporter_event', { data: 'test' });
```

## Building

### Android

```bash
cd android
./gradlew assembleDebug
```

### iOS

```bash
cd ios
xcodebuild -workspace YourApp.xcworkspace -scheme YourApp -configuration Debug
```

## Troubleshooting

### Common Issues

1. **CMake not found**: Ensure CMake 3.13+ is installed
2. **NDK version mismatch**: Use NDK 21.4.7075529
3. **Build errors**: Check that new architecture is enabled
4. **Module not found**: Verify module registration in OnLoad.cpp and AppDelegate.mm

### Debug Tips

- Enable verbose logging: `react-native run-android --verbose`
- Check generated files in `build/generated/`
- Verify spec files are correctly formatted
- Ensure all dependencies are properly linked

## Migration from Old Architecture

If migrating from the old architecture:

1. Remove old module imports
2. Update to new spec-based imports
3. Update method signatures to match new types
4. Test thoroughly on both platforms

## Support

For issues and questions:

- GitHub Issues: [AppMetrica React Native](https://github.com/appmetrica/appmetrica-react-native-plugin/issues)
- Documentation: [AppMetrica Docs](https://appmetrica.io/docs/)
