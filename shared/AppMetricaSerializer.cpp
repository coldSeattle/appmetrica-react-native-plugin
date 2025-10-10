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

    std::string AppMetricaSerializer::serializeConfig(const AppMetricaConfig &config)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"apiKey\":\"" << config.apiKey() << "\"";

        if (config.appVersion().has_value())
        {
            oss << ",\"appVersion\":\"" << config.appVersion().value() << "\"";
        }
        if (config.crashReporting().has_value())
        {
            oss << ",\"crashReporting\":" << (config.crashReporting().value() ? "true" : "false");
        }
        if (config.firstActivationAsUpdate().has_value())
        {
            oss << ",\"firstActivationAsUpdate\":" << (config.firstActivationAsUpdate().value() ? "true" : "false");
        }
        if (config.location().has_value())
        {
            oss << ",\"location\":" << serializeLocation(config.location().value());
        }
        if (config.locationTracking().has_value())
        {
            oss << ",\"locationTracking\":" << (config.locationTracking().value() ? "true" : "false");
        }
        if (config.logs().has_value())
        {
            oss << ",\"logs\":" << (config.logs().value() ? "true" : "false");
        }
        if (config.sessionTimeout().has_value())
        {
            oss << ",\"sessionTimeout\":" << config.sessionTimeout().value();
        }
        if (config.statisticsSending().has_value())
        {
            oss << ",\"statisticsSending\":" << (config.statisticsSending().value() ? "true" : "false");
        }
        if (config.preloadInfo().has_value())
        {
            oss << ",\"preloadInfo\":{\"trackingId\":\"" << config.preloadInfo().value().trackingId() << "\"}";
        }
        if (config.maxReportsInDatabaseCount().has_value())
        {
            oss << ",\"maxReportsInDatabaseCount\":" << config.maxReportsInDatabaseCount().value();
        }
        if (config.nativeCrashReporting().has_value())
        {
            oss << ",\"nativeCrashReporting\":" << (config.nativeCrashReporting().value() ? "true" : "false");
        }
        if (config.activationAsSessionStart().has_value())
        {
            oss << ",\"activationAsSessionStart\":" << (config.activationAsSessionStart().value() ? "true" : "false");
        }
        if (config.sessionsAutoTracking().has_value())
        {
            oss << ",\"sessionsAutoTracking\":" << (config.sessionsAutoTracking().value() ? "true" : "false");
        }
        if (config.appOpenTrackingEnabled().has_value())
        {
            oss << ",\"appOpenTrackingEnabled\":" << (config.appOpenTrackingEnabled().value() ? "true" : "false");
        }
        if (config.userProfileID().has_value())
        {
            oss << ",\"userProfileID\":\"" << config.userProfileID().value() << "\"";
        }
        if (config.errorEnvironment().has_value())
        {
            oss << ",\"errorEnvironment\":" << toJson(config.errorEnvironment().value());
        }
        if (config.appEnvironment().has_value())
        {
            oss << ",\"appEnvironment\":" << toJson(config.appEnvironment().value());
        }
        if (config.maxReportsCount().has_value())
        {
            oss << ",\"maxReportsCount\":" << config.maxReportsCount().value();
        }
        if (config.dispatchPeriodSeconds().has_value())
        {
            oss << ",\"dispatchPeriodSeconds\":" << config.dispatchPeriodSeconds().value();
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeLocation(const Location &location)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"latitude\":" << location.latitude();
        oss << ",\"longitude\":" << location.longitude();

        if (location.altitude().has_value())
        {
            oss << ",\"altitude\":" << location.altitude().value();
        }
        if (location.accuracy().has_value())
        {
            oss << ",\"accuracy\":" << location.accuracy().value();
        }
        if (location.course().has_value())
        {
            oss << ",\"course\":" << location.course().value();
        }
        if (location.speed().has_value())
        {
            oss << ",\"speed\":" << location.speed().value();
        }
        if (location.timestamp().has_value())
        {
            oss << ",\"timestamp\":" << location.timestamp().value();
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeECommerceEvent(const ECommerceEvent &event)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"ecommerceEvent\":\"" << event.ecommerceEvent() << "\"";

        if (event.ecommerceScreen().has_value())
        {
            oss << ",\"ecommerceScreen\":" << "[Object]"; // Simplified for now
        }
        if (event.product().has_value())
        {
            oss << ",\"product\":" << "[Object]"; // Simplified for now
        }
        if (event.referrer().has_value())
        {
            oss << ",\"referrer\":" << "[Object]"; // Simplified for now
        }
        if (event.cartItem().has_value())
        {
            oss << ",\"cartItem\":" << "[Object]"; // Simplified for now
        }
        if (event.order().has_value())
        {
            oss << ",\"order\":" << "[Object]"; // Simplified for now
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeRevenue(const Revenue &revenue)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"price\":" << revenue.price();
        oss << ",\"currency\":\"" << revenue.currency() << "\"";

        if (revenue.productID().has_value())
        {
            oss << ",\"productID\":\"" << revenue.productID().value() << "\"";
        }
        if (revenue.quantity().has_value())
        {
            oss << ",\"quantity\":" << revenue.quantity().value();
        }
        if (revenue.payload().has_value())
        {
            oss << ",\"payload\":\"" << revenue.payload().value() << "\"";
        }
        if (revenue.receipt().has_value())
        {
            oss << ",\"receipt\":" << "[Object]"; // Simplified for now
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeAdRevenue(const AdRevenue &adRevenue)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"price\":\"" << adRevenue.price() << "\"";
        oss << ",\"currency\":\"" << adRevenue.currency() << "\"";

        if (adRevenue.payload().has_value())
        {
            oss << ",\"payload\":" << "[Object]"; // Simplified for now
        }
        if (adRevenue.adNetwork().has_value())
        {
            oss << ",\"adNetwork\":\"" << adRevenue.adNetwork().value() << "\"";
        }
        if (adRevenue.adPlacementID().has_value())
        {
            oss << ",\"adPlacementID\":\"" << adRevenue.adPlacementID().value() << "\"";
        }
        if (adRevenue.adPlacementName().has_value())
        {
            oss << ",\"adPlacementName\":\"" << adRevenue.adPlacementName().value() << "\"";
        }
        if (adRevenue.adType().has_value())
        {
            oss << ",\"adType\":\"" << adRevenue.adType().value() << "\"";
        }
        if (adRevenue.adUnitID().has_value())
        {
            oss << ",\"adUnitID\":\"" << adRevenue.adUnitID().value() << "\"";
        }
        if (adRevenue.adUnitName().has_value())
        {
            oss << ",\"adUnitName\":\"" << adRevenue.adUnitName().value() << "\"";
        }
        if (adRevenue.precision().has_value())
        {
            oss << ",\"precision\":\"" << adRevenue.precision().value() << "\"";
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeUserProfile(const UserProfile &userProfile)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"attributes\":" << "[Array]"; // Simplified for now
        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeError(const AppMetricaError &error)
    {
        std::ostringstream oss;
        oss << "{";

        if (error.identifier().has_value())
        {
            oss << "\"identifier\":\"" << error.identifier().value() << "\"";
        }
        if (error.message().has_value())
        {
            oss << ",\"message\":\"" << error.message().value() << "\"";
        }
        if (error.stacktrace().has_value())
        {
            oss << ",\"stacktrace\":" << "[Array]"; // Simplified for now
        }
        if (error.platform().has_value())
        {
            oss << ",\"platform\":\"" << error.platform().value() << "\"";
        }
        if (error.virtualMachineVersion().has_value())
        {
            oss << ",\"virtualMachineVersion\":\"" << error.virtualMachineVersion().value() << "\"";
        }
        if (error.buildId().has_value())
        {
            oss << ",\"buildId\":\"" << error.buildId().value() << "\"";
        }
        if (error.buildType().has_value())
        {
            oss << ",\"buildType\":\"" << error.buildType().value() << "\"";
        }
        if (error.multidex().has_value())
        {
            oss << ",\"multidex\":" << (error.multidex().value() ? "true" : "false");
        }
        if (error.threads().has_value())
        {
            oss << ",\"threads\":" << "[Array]"; // Simplified for now
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeExternalAttribution(const ExternalAttribution &attribution)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"source\":\"" << attribution.source() << "\"";
        oss << ",\"value\":" << "[Object]"; // Simplified for now
        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeReporterConfig(const ReporterConfig &config)
    {
        std::ostringstream oss;
        oss << "{";
        oss << "\"apiKey\":\"" << config.apiKey() << "\"";

        if (config.logs().has_value())
        {
            oss << ",\"logs\":" << (config.logs().value() ? "true" : "false");
        }
        if (config.maxReportsInDatabaseCount().has_value())
        {
            oss << ",\"maxReportsInDatabaseCount\":" << config.maxReportsInDatabaseCount().value();
        }
        if (config.sessionTimeout().has_value())
        {
            oss << ",\"sessionTimeout\":" << config.sessionTimeout().value();
        }
        if (config.dataSendingEnabled().has_value())
        {
            oss << ",\"dataSendingEnabled\":" << (config.dataSendingEnabled().value() ? "true" : "false");
        }
        if (config.appEnvironment().has_value())
        {
            oss << ",\"appEnvironment\":" << "[Object]"; // Simplified for now
        }
        if (config.dispatchPeriodSeconds().has_value())
        {
            oss << ",\"dispatchPeriodSeconds\":" << config.dispatchPeriodSeconds().value();
        }
        if (config.userProfileID().has_value())
        {
            oss << ",\"userProfileID\":\"" << config.userProfileID().value() << "\"";
        }
        if (config.maxReportsCount().has_value())
        {
            oss << ",\"maxReportsCount\":" << config.maxReportsCount().value();
        }

        oss << "}";
        return oss.str();
    }

    std::string AppMetricaSerializer::serializeStartupParams(const StartupParams &params)
    {
        std::ostringstream oss;
        oss << "{";

        if (params.deviceIdHash().has_value())
        {
            oss << "\"deviceIdHash\":\"" << params.deviceIdHash().value() << "\"";
        }
        if (params.deviceId().has_value())
        {
            oss << ",\"deviceId\":\"" << params.deviceId().value() << "\"";
        }
        if (params.uuid().has_value())
        {
            oss << ",\"uuid\":\"" << params.uuid().value() << "\"";
        }

        oss << "}";
        return oss.str();
    }

    // Helper methods for JSON serialization
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

    // Deserialization methods (simplified implementations)
    AppMetricaConfig AppMetricaSerializer::deserializeConfig(const std::string &json)
    {
        // Simplified implementation - would need proper JSON parsing
        return AppMetricaConfig{};
    }

    ECommerceEvent AppMetricaSerializer::deserializeECommerceEvent(const std::string &json)
    {
        return ECommerceEvent{};
    }

    Revenue AppMetricaSerializer::deserializeRevenue(const std::string &json)
    {
        return Revenue{};
    }

    AdRevenue AppMetricaSerializer::deserializeAdRevenue(const std::string &json)
    {
        return AdRevenue{};
    }

    UserProfile AppMetricaSerializer::deserializeUserProfile(const std::string &json)
    {
        return UserProfile{};
    }

    AppMetricaError AppMetricaSerializer::deserializeError(const std::string &json)
    {
        return AppMetricaError{};
    }

    ExternalAttribution AppMetricaSerializer::deserializeExternalAttribution(const std::string &json)
    {
        return ExternalAttribution{};
    }

    ReporterConfig AppMetricaSerializer::deserializeReporterConfig(const std::string &json)
    {
        return ReporterConfig{};
    }

    Location AppMetricaSerializer::deserializeLocation(const std::string &json)
    {
        return Location{};
    }

    StartupParams AppMetricaSerializer::deserializeStartupParams(const std::string &json)
    {
        return StartupParams{};
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
