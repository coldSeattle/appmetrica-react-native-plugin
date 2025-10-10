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
            std::optional<NativeAppMetricaReporterAppMetricaError> reason);

        void reportErrorWithoutIdentifier(
            std::string apiKey,
            std::string message,
            NativeAppMetricaReporterAppMetricaError error);

        void reportUnhandledException(
            std::string apiKey,
            NativeAppMetricaReporterAppMetricaError error);

        // Event reporting
        void reportEvent(
            std::string apiKey,
            std::string eventName,
            std::optional<jsi::Object> attributes);

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
        void setUserProfileID(std::string apiKey, std::optional<std::string> userProfileID);
        void setDataSendingEnabled(std::string apiKey, bool enabled);
        void reportUserProfile(std::string apiKey, NativeAppMetricaReporterUserProfile userProfile);

        // Revenue reporting
        void reportAdRevenue(std::string apiKey, NativeAppMetricaReporterAdRevenue adRevenue);
        void reportECommerce(std::string apiKey, NativeAppMetricaReporterECommerceEvent event);
        void reportRevenue(std::string apiKey, NativeAppMetricaReporterRevenue revenue);

    private:
        // Helper methods for data conversion
        std::map<std::string, std::string> convertObjectToStringMap(jsi::Runtime &runtime, const jsi::Object &obj);
        std::vector<std::string> convertArrayToStringVector(jsi::Runtime &runtime, const jsi::Array &arr);
    };

} // namespace facebook::react
