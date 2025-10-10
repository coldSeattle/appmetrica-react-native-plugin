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

    private:
        // Helper methods for JSON serialization/deserialization
        static std::string toJson(const std::map<std::string, std::string> &map);
        static std::string toJson(const std::vector<std::string> &vec);
        static std::map<std::string, std::string> fromJsonMap(const std::string &json);
        static std::vector<std::string> fromJsonArray(const std::string &json);
    };

} // namespace facebook::react