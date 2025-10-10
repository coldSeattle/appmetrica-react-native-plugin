#pragma once
#include <AppMetricaSpecsJSI.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace facebook::react
{

    class NativeAppMetricaReporter : public NativeAppMetricaReporterSpecCxxSpec<NativeAppMetricaReporter>
    {
    public:
        NativeAppMetricaReporter(std::shared_ptr<CallInvoker> jsInvoker);

        // Error reporting
        void reportError(
            std::string apiKey,
            std::string identifier,
            std::optional<std::string> message,
            std::optional<AppMetricaError> reason);

        void reportErrorWithoutIdentifier(
            std::string apiKey,
            std::string message,
            AppMetricaError error);

        void reportUnhandledException(
            std::string apiKey,
            AppMetricaError error);

        // Event reporting
        void reportEvent(
            std::string apiKey,
            std::string eventName,
            std::optional<Object> attributes);

        // Session management
        void pauseSession(std::string apiKey);
        void resumeSession(std::string apiKey);
        void sendEventsBuffer(std::string apiKey);

        // Environment
        void clearAppEnvironment(std::string apiKey);
        void putAppEnvironmentValue(
            std::string apiKey,
            std::string key,
            std::optional<std::string> value);

        // User profile
        void setUserProfileID(std::string apiKey, std::string userProfileID);
        void setDataSendingEnabled(std::string apiKey, bool enabled);
        void reportUserProfile(std::string apiKey, UserProfile userProfile);

        // Revenue reporting
        void reportAdRevenue(std::string apiKey, AdRevenue adRevenue);
        void reportECommerce(std::string apiKey, ECommerceEvent event);
        void reportRevenue(std::string apiKey, Revenue revenue);

    private:
        // Helper methods for data conversion
        std::map<std::string, std::string> convertObjectToStringMap(Object obj);
        std::vector<std::string> convertArrayToStringVector(std::vector<Object> arr);

        // Platform-specific implementations
        void reportErrorAndroid(std::string apiKey, std::string identifier, std::optional<std::string> message, std::optional<AppMetricaError> reason);
        void reportErrorIOS(std::string apiKey, std::string identifier, std::optional<std::string> message, std::optional<AppMetricaError> reason);

        void reportEventAndroid(std::string apiKey, std::string eventName, std::optional<Object> attributes);
        void reportEventIOS(std::string apiKey, std::string eventName, std::optional<Object> attributes);
    };

} // namespace facebook::react
