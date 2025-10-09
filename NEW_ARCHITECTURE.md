# AppMetrica React Native Plugin - New Architecture Support

This library now supports React Native's New Architecture (Turbo Modules) for iOS.

## Requirements

- React Native 0.68+
- iOS 11.0+
- Xcode 14+

## Installation

### 1. Install the package

```bash
npm install @appmetrica/react-native-analytics
# or
yarn add @appmetrica/react-native-analytics
```

### 2. iOS Setup

#### For New Architecture (Turbo Modules)

1. Enable the new architecture in your `ios/Podfile`:

```ruby
# Add this line at the top of your Podfile
use_frameworks! :linkage => :static

# Enable new architecture
ENV['RCT_NEW_ARCH_ENABLED'] = '1'
```

2. Run pod install:

```bash
cd ios && pod install
```

3. Clean and rebuild your project:

```bash
# Clean
cd ios && xcodebuild clean

# Rebuild
npx react-native run-ios
```

#### For Legacy Architecture

If you're not using the new architecture, the library will automatically fall back to the legacy implementation.

## Usage

The API remains the same regardless of whether you're using the new architecture or legacy:

```typescript
import AppMetrica from '@appmetrica/react-native-analytics';

// Initialize AppMetrica
AppMetrica.activate({
  apiKey: 'YOUR_API_KEY',
  // ... other config options
});

// Report events
AppMetrica.reportEvent('test_event', { key: 'value' });
```

## Migration Guide

### From Legacy to New Architecture

1. Update your `ios/Podfile` as shown above
2. Run `pod install`
3. Clean and rebuild your project
4. No code changes are required - the library handles the transition automatically

### Troubleshooting

#### Common Issues

1. **Build errors after enabling new architecture:**

   - Clean your project: `cd ios && xcodebuild clean`
   - Delete `ios/build` folder
   - Run `pod install` again
   - Rebuild the project

2. **Turbo Module not found:**

   - Ensure `ENV['RCT_NEW_ARCH_ENABLED'] = '1'` is set in your Podfile
   - Check that you're using React Native 0.68+
   - Verify that `use_frameworks! :linkage => :static` is in your Podfile

3. **Legacy module fallback:**
   - This is normal behavior when new architecture is not enabled
   - The library will automatically use the legacy implementation

## Architecture Details

### Turbo Module Implementation

The library includes a Turbo Module implementation (`AppMetricaTurboModule`) that provides:

- Better performance
- Type safety
- Improved memory management
- Better error handling

### Codegen Configuration

The library includes proper `codegenConfig` in `package.json`:

```json
{
  "codegenConfig": {
    "name": "AppMetricaTurboSpec",
    "type": "modules",
    "jsSrcsDir": "src"
  }
}
```

This configuration tells React Native's Codegen tool:
- Where to find the spec file (`src/AppMetricaTurboSpec.ts`)
- What type of module it is (`modules`)
- The name of the spec (`AppMetricaTurboSpec`)

### Automatic Fallback

The library automatically detects whether the new architecture is available and falls back to the legacy implementation if needed. This ensures compatibility across different React Native versions and configurations.

## Support

For issues related to the new architecture support, please check:

1. React Native's official documentation on Turbo Modules
2. This library's GitHub issues
3. AppMetrica's official documentation

## Version Compatibility

| React Native Version | New Architecture Support | Legacy Support |
| -------------------- | ------------------------ | -------------- |
| 0.68+                | ✅ Yes                   | ✅ Yes         |
| 0.67 and below       | ❌ No                    | ✅ Yes         |
