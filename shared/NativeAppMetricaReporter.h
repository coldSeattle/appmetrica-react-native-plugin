#pragma once
#include <AppMetricaSpecsJSI.h>
#include <jsi/jsi.h>
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
            jsi::Runtime &runtime,
            std::string apiKey,
            std::string identifier,
            std::optional<std::string> message,
            std::optional<NativeAppMetricaReporterSpecAppMetricaError> reason);

        void reportErrorWithoutIdentifier(
            jsi::Runtime &runtime,
            std::string apiKey,
            std::string message,
            NativeAppMetricaReporterSpecAppMetricaError error);

        void reportUnhandledException(
            jsi::Runtime &runtime,
            std::string apiKey,
            NativeAppMetricaReporterSpecAppMetricaError error);

        // Event reporting
        void reportEvent(
            jsi::Runtime &runtime,
            std::string apiKey,
            std::string eventName,
            std::optional<jsi::Object> attributes);

        // Session management
        void pauseSession(jsi::Runtime &runtime, std::string apiKey);
        void resumeSession(jsi::Runtime &runtime, std::string apiKey);
        void sendEventsBuffer(jsi::Runtime &runtime, std::string apiKey);

        // Environment
        void clearAppEnvironment(jsi::Runtime &runtime, std::string apiKey);
        void putAppEnvironmentValue(
            jsi::Runtime &runtime,
            std::string apiKey,
            std::string key,
            std::optional<std::string> value);

        // User profile
        void setUserProfileID(jsi::Runtime &runtime, std::string apiKey, std::optional<std::string> userProfileID);
        void setDataSendingEnabled(jsi::Runtime &runtime, std::string apiKey, bool enabled);
        void reportUserProfile(jsi::Runtime &runtime, std::string apiKey, NativeAppMetricaReporterSpecUserProfile userProfile);

        // Revenue reporting
        void reportAdRevenue(jsi::Runtime &runtime, std::string apiKey, NativeAppMetricaReporterSpecAdRevenue adRevenue);
        void reportECommerce(jsi::Runtime &runtime, std::string apiKey, NativeAppMetricaReporterSpecECommerceEvent event);
        void reportRevenue(jsi::Runtime &runtime, std::string apiKey, NativeAppMetricaReporterSpecRevenue revenue);

    private:
        // Helper methods for data conversion
        std::map<std::string, std::string> convertObjectToStringMap(jsi::Runtime &runtime, const jsi::Object &obj);
        std::vector<std::string> convertArrayToStringVector(jsi::Runtime &runtime, const jsi::Array &arr);
    };

} // namespace facebook::react
