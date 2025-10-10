#include "AppMetricaSerializer.h"
#include <sstream>
#include <iostream>

namespace facebook::react
{

    std::map<std::string, std::string> AppMetricaSerializer::objectToStringMap(jsi::Runtime &runtime, const jsi::Object &obj)
    {
        std::map<std::string, std::string> result;

        auto propertyNames = obj.getPropertyNames(runtime);
        for (size_t i = 0; i < propertyNames.size(runtime); i++)
        {
            auto key = propertyNames.getValueAtIndex(runtime, i).asString(runtime).utf8(runtime);
            auto value = obj.getProperty(runtime, key.c_str());
            result[key] = valueToString(runtime, value);
        }

        return result;
    }

    std::vector<std::string> AppMetricaSerializer::arrayToStringVector(jsi::Runtime &runtime, const jsi::Array &arr)
    {
        std::vector<std::string> result;

        for (size_t i = 0; i < arr.size(runtime); i++)
        {
            auto value = arr.getValueAtIndex(runtime, i);
            result.push_back(valueToString(runtime, value));
        }

        return result;
    }

    std::string AppMetricaSerializer::valueToString(jsi::Runtime &runtime, const jsi::Value &value)
    {
        if (value.isString())
        {
            return value.asString(runtime).utf8(runtime);
        }
        else if (value.isNumber())
        {
            return std::to_string(value.asNumber());
        }
        else if (value.isBool())
        {
            return value.asBool() ? "true" : "false";
        }
        else if (value.isNull() || value.isUndefined())
        {
            return "";
        }
        else
        {
            return "[Object]"; // For complex objects, we'll need more sophisticated serialization
        }
    }

    jsi::Object AppMetricaSerializer::stringMapToObject(jsi::Runtime &runtime, const std::map<std::string, std::string> &map)
    {
        jsi::Object result(runtime);

        for (const auto &pair : map)
        {
            result.setProperty(runtime, pair.first.c_str(), jsi::String::createFromUtf8(runtime, pair.second));
        }

        return result;
    }

    jsi::Array AppMetricaSerializer::stringVectorToArray(jsi::Runtime &runtime, const std::vector<std::string> &vec)
    {
        jsi::Array result(runtime, vec.size());

        for (size_t i = 0; i < vec.size(); i++)
        {
            result.setValueAtIndex(runtime, i, jsi::String::createFromUtf8(runtime, vec[i]));
        }

        return result;
    }

    // Helper methods for JSON serialization/deserialization
    std::string AppMetricaSerializer::toJson(const std::map<std::string, std::string> &map)
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto &pair : map)
        {
            if (!first)
                oss << ",";
            oss << "\"" << pair.first << "\":\"" << pair.second << "\"";
            first = false;
        }
        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::toJson(const std::vector<std::string> &vec)
    {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (i > 0)
                oss << ",";
            oss << "\"" << vec[i] << "\"";
        }
        oss << "]";
        return oss.str();
    }

    std::map<std::string, std::string> AppMetricaSerializer::fromJsonMap(const std::string &json)
    {
        // Simplified implementation - would need proper JSON parsing
        return std::map<std::string, std::string>{};
    }

    std::vector<std::string> AppMetricaSerializer::fromJsonArray(const std::string &json)
    {
        // Simplified implementation - would need proper JSON parsing
        return std::vector<std::string>{};
    }

} // namespace facebook::react