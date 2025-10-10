#pragma once
#include <jsi/jsi.h>
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace facebook::react
{

    class AppMetricaSerializer
    {
    public:
        // Convert JSI Object to string map
        static std::map<std::string, std::string> objectToStringMap(jsi::Runtime &runtime, const jsi::Object &obj);

        // Convert JSI Array to string vector
        static std::vector<std::string> arrayToStringVector(jsi::Runtime &runtime, const jsi::Array &arr);

        // Convert JSI Value to string
        static std::string valueToString(jsi::Runtime &runtime, const jsi::Value &value);

        // Convert string map to JSI Object
        static jsi::Object stringMapToObject(jsi::Runtime &runtime, const std::map<std::string, std::string> &map);

        // Convert string vector to JSI Array
        static jsi::Array stringVectorToArray(jsi::Runtime &runtime, const std::vector<std::string> &vec);

        // Serialize AppMetricaConfig for platform calls
        static std::string serializeConfig(const AppMetricaConfig &config);

        // Serialize ECommerceEvent for platform calls
        static std::string serializeECommerceEvent(const ECommerceEvent &event);

        // Serialize Revenue for platform calls
        static std::string serializeRevenue(const Revenue &revenue);

        // Serialize AdRevenue for platform calls
        static std::string serializeAdRevenue(const AdRevenue &adRevenue);

        // Serialize UserProfile for platform calls
        static std::string serializeUserProfile(const UserProfile &userProfile);

        // Serialize AppMetricaError for platform calls
        static std::string serializeError(const AppMetricaError &error);

        // Serialize ExternalAttribution for platform calls
        static std::string serializeExternalAttribution(const ExternalAttribution &attribution);

        // Serialize ReporterConfig for platform calls
        static std::string serializeReporterConfig(const ReporterConfig &config);

        // Serialize Location for platform calls
        static std::string serializeLocation(const Location &location);

        // Serialize StartupParams for platform calls
        static std::string serializeStartupParams(const StartupParams &params);

        // Deserialize responses from platform calls
        static AppMetricaConfig deserializeConfig(const std::string &json);
        static ECommerceEvent deserializeECommerceEvent(const std::string &json);
        static Revenue deserializeRevenue(const std::string &json);
        static AdRevenue deserializeAdRevenue(const std::string &json);
        static UserProfile deserializeUserProfile(const std::string &json);
        static AppMetricaError deserializeError(const std::string &json);
        static ExternalAttribution deserializeExternalAttribution(const std::string &json);
        static ReporterConfig deserializeReporterConfig(const std::string &json);
        static Location deserializeLocation(const std::string &json);
        static StartupParams deserializeStartupParams(const std::string &json);

    private:
        // Helper methods for JSON serialization/deserialization
        static std::string toJson(const std::map<std::string, std::string> &map);
        static std::string toJson(const std::vector<std::string> &vec);
        static std::map<std::string, std::string> fromJsonMap(const std::string &json);
        static std::vector<std::string> fromJsonArray(const std::string &json);
    };

} // namespace facebook::react
