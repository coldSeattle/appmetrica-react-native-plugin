#pragma once
#include <AppMetricaSpecsJSI.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace facebook::react
{

    class NativeAppMetrica : public NativeAppMetricaSpecCxxSpec<NativeAppMetrica>
    {
    public:
        NativeAppMetrica(std::shared_ptr<CallInvoker> jsInvoker);

        // Configuration
        void activate(NativeAppMetricaSpecAppMetricaConfig config);

        // Library info
        double getLibraryApiLevel();
        std::string getLibraryVersion();

        // Session management
        void pauseSession();
        void resumeSession();
        void sendEventsBuffer();

        // App tracking
        void reportAppOpen(std::optional<std::string> deeplink);

        // Error reporting
        void reportError(std::string identifier, std::optional<std::string> message, std::optional<NativeAppMetricaSpecAppMetricaError> reason);
        void reportUnhandledException(NativeAppMetricaSpecAppMetricaError error);
        void reportErrorWithoutIdentifier(std::string message, NativeAppMetricaSpecAppMetricaError error);

        // Event reporting
        void reportEvent(std::string eventName, std::optional<jsi::Object> attributes);

        // Location
        void setLocation(std::optional<NativeAppMetricaSpecLocation> location);
        void setLocationTracking(bool enabled);

        // Data sending
        void setDataSendingEnabled(bool enabled);

        // User profile
        void setUserProfileID(std::optional<std::string> userProfileID);
        void reportUserProfile(NativeAppMetricaSpecUserProfile userProfile);

        // ECommerce
        void reportECommerce(NativeAppMetricaSpecECommerceEvent event);

        // Revenue
        void reportRevenue(NativeAppMetricaSpecRevenue revenue);
        void reportAdRevenue(NativeAppMetricaSpecAdRevenue adRevenue);

        // Environment
        void putErrorEnvironmentValue(std::string key, std::optional<std::string> value);
        void putAppEnvironmentValue(std::string key, std::optional<std::string> value);
        void clearAppEnvironment();

        // External Attribution
        void reportExternalAttribution(NativeAppMetricaSpecExternalAttribution attribution);

        // Reporter management
        void activateReporter(NativeAppMetricaSpecReporterConfig config);
        void touchReporter(std::string apiKey);

        // Device info
        std::string getDeviceId();
        std::string getUuid();

        // Startup params
        void requestStartupParams(std::vector<std::string> identifiers, std::function<void(std::optional<NativeAppMetricaSpecStartupParams>, std::optional<std::string>)> callback);

        // Deferred deeplinks
        void requestDeferredDeeplink(
            std::function<void(std::string, std::optional<std::string>)> onFailure,
            std::function<void(std::string)> onSuccess);

        void requestDeferredDeeplinkParameters(
            std::function<void(std::string, std::optional<std::string>)> onFailure,
            std::function<void(jsi::Object)> onSuccess);

    private:
        // Helper methods for data conversion
        std::map<std::string, std::string> convertObjectToStringMap(jsi::Runtime &runtime, const jsi::Object &obj);
        std::vector<std::string> convertArrayToStringVector(jsi::Runtime &runtime, const jsi::Array &arr);

        // Platform-specific implementations
        void activateAndroid(NativeAppMetricaSpecAppMetricaConfig config);
        void activateIOS(NativeAppMetricaSpecAppMetricaConfig config);
    };

} // namespace facebook::react