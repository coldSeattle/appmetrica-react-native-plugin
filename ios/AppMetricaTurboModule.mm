#import <React/RCTBridgeModule.h>
#import <ReactCommon/RCTTurboModule.h>

// Импортируем ваши существующие утилиты
#import "AMARNAppMetrica.h"
#import "AMARNAppMetricaUtils.h"
#import "AMARNStartupParamsUtils.h"
#import "AMARNUserProfileSerializer.h"
#import "AMARNExternalAttribution.h"
#import "AMARNExceptionSerializer.h"

/**
 * AppMetricaTurboModule - iOS реализация для новой архитектуры React Native
 * 
 * Этот класс реализует интерфейс TurboModule для AppMetrica
 * Позволяет JavaScript коду напрямую вызывать нативные методы через JSI
 * без использования старого Bridge с JSON сериализацией
 * 
 * Все методы соответствуют спецификации в AppMetricaSpec.ts
 */
@interface AppMetricaTurboModule : NSObject <RCTTurboModule>

@end

@implementation AppMetricaTurboModule

// Реализуем протокол RCTTurboModule
RCT_EXPORT_MODULE(AppMetrica)

// MARK: - Основные методы активации и управления

/**
 * Активация AppMetrica с конфигурацией
 * Вызывается напрямую из JavaScript через JSI
 * 
 * @param config - словарь с конфигурацией AppMetrica
 */
RCT_EXPORT_METHOD(activate:(NSDictionary *)config)
{
    // Используем ваши существующие утилиты для конвертации конфигурации
    [[AMAAppMetricaCrashes crashes] setConfiguration:[AMARNAppMetricaUtils crashConfigurationForDictionary:config]];
    [AMAAppMetrica activateWithConfiguration:[AMARNAppMetricaUtils configurationForDictionary:config]];
}

/**
 * Получение API уровня библиотеки (только для Android)
 * В iOS этот метод не реализован, возвращаем 0
 * 
 * @return NSNumber с значением 0
 */
RCT_EXPORT_METHOD(getLibraryApiLevel:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    resolve(@0); // iOS не поддерживает API level
}

/**
 * Получение версии библиотеки
 * Возвращаем версию библиотеки AppMetrica
 * 
 * @return NSString с версией библиотеки
 */
RCT_EXPORT_METHOD(getLibraryVersion:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    resolve([AMAAppMetrica libraryVersion]);
}

// MARK: - Управление сессиями

/**
 * Приостановка сессии
 * Вызываем нативный метод AppMetrica
 */
RCT_EXPORT_METHOD(pauseSession)
{
    [AMAAppMetrica pauseSession];
}

/**
 * Возобновление сессии
 * Вызываем нативный метод AppMetrica
 */
RCT_EXPORT_METHOD(resumeSession)
{
    [AMAAppMetrica resumeSession];
}

/**
 * Отправка буфера событий
 * Вызываем нативный метод AppMetrica
 */
RCT_EXPORT_METHOD(sendEventsBuffer)
{
    [AMAAppMetrica sendEventsBuffer];
}

// MARK: - Отчеты о событиях и ошибках

/**
 * Отчет об открытии приложения
 * Конвертируем deeplink в URL и вызываем нативный метод
 * 
 * @param deeplink - строка с deeplink (может быть nil)
 */
RCT_EXPORT_METHOD(reportAppOpen:(NSString *)deeplink)
{
    if (deeplink) {
        [AMAAppMetrica trackOpeningURL:[NSURL URLWithString:deeplink]];
    }
}

/**
 * Отчет о событии
 * Вызываем нативный метод AppMetrica с параметрами
 * 
 * @param eventName - название события
 * @param attributes - словарь с атрибутами события (может быть nil)
 */
RCT_EXPORT_METHOD(reportEvent:(NSString *)eventName attributes:(NSDictionary *)attributes)
{
    if (attributes) {
        [AMAAppMetrica reportEvent:eventName parameters:attributes onFailure:^(NSError *error) {
            NSLog(@"AppMetrica error: %@", [error localizedDescription]);
        }];
    } else {
        [AMAAppMetrica reportEvent:eventName onFailure:^(NSError *error) {
            NSLog(@"AppMetrica error: %@", [error localizedDescription]);
        }];
    }
}

/**
 * Отчет об ошибке
 * Вызываем нативный метод AppMetrica с деталями ошибки
 * 
 * @param identifier - идентификатор ошибки
 * @param message - сообщение об ошибке (может быть nil)
 * @param reason - словарь с деталями ошибки (может быть nil)
 */
RCT_EXPORT_METHOD(reportError:(NSString *)identifier message:(NSString *)message reason:(NSDictionary *)reason)
{
    [[[AMAAppMetricaCrashes crashes] pluginExtension] reportErrorWithIdentifier:identifier
                                                                        message:message
                                                                        details:amarn_exceptionForDictionary(reason)
                                                                      onFailure:^(NSError *error) {
        NSLog(@"Failed to report error to AppMetrica: %@", [error localizedDescription]);
    }];
}

/**
 * Отчет об ошибке без идентификатора
 * Вызываем нативный метод AppMetrica с деталями ошибки
 * 
 * @param message - сообщение об ошибке
 * @param error - словарь с деталями ошибки
 */
RCT_EXPORT_METHOD(reportErrorWithoutIdentifier:(NSString *)message error:(NSDictionary *)error)
{
    AMAPluginErrorDetails *details = amarn_exceptionForDictionary(error);
    if (details.backtrace.count == 0) {
        [[[AMAAppMetricaCrashes crashes] pluginExtension] reportErrorWithIdentifier:@"Errors without stacktrace"
                                                                            message:message
                                                                            details:details
                                                                          onFailure:^(NSError *err) {
            NSLog(@"Failed to report error without identifier to AppMetrica: %@", [err localizedDescription]);
        }];
    } else {
        [[[AMAAppMetricaCrashes crashes] pluginExtension] reportError:details message:message onFailure:^(NSError *err) {
            NSLog(@"Failed to report error without identifier to AppMetrica: %@", [err localizedDescription]);
        }];
    }
}

/**
 * Отчет о необработанном исключении
 * Вызываем нативный метод AppMetrica с деталями исключения
 * 
 * @param error - словарь с деталями исключения
 */
RCT_EXPORT_METHOD(reportUnhandledException:(NSDictionary *)error)
{
    [[[AMAAppMetricaCrashes crashes] pluginExtension] reportUnhandledException:amarn_exceptionForDictionary(error)
                                                                   onFailure:^(NSError *err) {
        NSLog(@"Failed to report unhandled exception to AppMetrica: %@", [err localizedDescription]);
    }];
}

// MARK: - E-commerce события

/**
 * Отчет о E-commerce событии
 * Вызываем нативный метод AppMetrica с E-commerce событием
 * 
 * @param ecommerceEvent - словарь с E-commerce событием
 */
RCT_EXPORT_METHOD(reportECommerce:(NSDictionary *)ecommerceEvent)
{
    [AMAAppMetrica reportECommerce:[AMARNAppMetricaUtils ecommerceForDict:ecommerceEvent] onFailure:^(NSError *error) {
        NSLog(@"Failed to report E-commerce event to AppMetrica: %@", [error localizedDescription]);
    }];
}

/**
 * Отчет о доходе
 * Вызываем нативный метод AppMetrica с доходом
 * 
 * @param revenue - словарь с доходом
 */
RCT_EXPORT_METHOD(reportRevenue:(NSDictionary *)revenue)
{
    [AMAAppMetrica reportRevenue:[AMARNAppMetricaUtils revenueForDict:revenue] onFailure:^(NSError *error) {
        NSLog(@"Failed to report revenue to AppMetrica: %@", [error localizedDescription]);
    }];
}

/**
 * Отчет о доходе от рекламы
 * Вызываем нативный метод AppMetrica с доходом от рекламы
 * 
 * @param adRevenue - словарь с доходом от рекламы
 */
RCT_EXPORT_METHOD(reportAdRevenue:(NSDictionary *)adRevenue)
{
    [AMAAppMetrica reportAdRevenue:[AMARNAppMetricaUtils adRevenueForDict:adRevenue] onFailure:^(NSError *error) {
        NSLog(@"Failed to report ad revenue to AppMetrica: %@", [error localizedDescription]);
    }];
}

// MARK: - Пользовательские данные

/**
 * Отчет о профиле пользователя
 * Вызываем нативный метод AppMetrica с профилем пользователя
 * 
 * @param userProfile - словарь с профилем пользователя
 */
RCT_EXPORT_METHOD(reportUserProfile:(NSDictionary *)userProfile)
{
    [AMAAppMetrica reportUserProfile:[AMARNAppMetricaUtils userProfileForDict:userProfile] onFailure:^(NSError *error) {
        NSLog(@"Failed to report user profile to AppMetrica: %@", [error localizedDescription]);
    }];
}

/**
 * Установка ID профиля пользователя
 * Вызываем нативный метод AppMetrica
 * 
 * @param userProfileID - ID профиля пользователя (может быть nil)
 */
RCT_EXPORT_METHOD(setUserProfileID:(NSString *)userProfileID)
{
    [AMAAppMetrica setUserProfileID:userProfileID];
}

// MARK: - Локация

/**
 * Установка локации
 * Вызываем нативный метод AppMetrica с локацией
 * 
 * @param location - словарь с локацией (может быть nil)
 */
RCT_EXPORT_METHOD(setLocation:(NSDictionary *)location)
{
    AMAAppMetrica.customLocation = [AMARNAppMetricaUtils locationForDictionary:location];
}

/**
 * Включение/выключение отслеживания локации
 * Вызываем нативный метод AppMetrica
 * 
 * @param enabled - включить отслеживание локации
 */
RCT_EXPORT_METHOD(setLocationTracking:(BOOL)enabled)
{
    AMAAppMetrica.locationTrackingEnabled = enabled;
}

// MARK: - Настройки данных

/**
 * Включение/выключение отправки данных
 * Вызываем нативный метод AppMetrica
 * 
 * @param enabled - включить отправку данных
 */
RCT_EXPORT_METHOD(setDataSendingEnabled:(BOOL)enabled)
{
    [AMAAppMetrica setDataSendingEnabled:enabled];
}

/**
 * Установка значения переменной окружения для ошибок
 * Вызываем нативный метод AppMetrica
 * 
 * @param key - ключ переменной окружения
 * @param value - значение переменной окружения (может быть nil)
 */
RCT_EXPORT_METHOD(putErrorEnvironmentValue:(NSString *)key value:(NSString *)value)
{
    [[AMAAppMetricaCrashes crashes] setErrorEnvironmentValue:value forKey:key];
}

/**
 * Установка значения переменной окружения приложения
 * Вызываем нативный метод AppMetrica
 * 
 * @param key - ключ переменной окружения
 * @param value - значение переменной окружения (может быть nil)
 */
RCT_EXPORT_METHOD(putAppEnvironmentValue:(NSString *)key value:(NSString *)value)
{
    [AMAAppMetrica setAppEnvironmentValue:value forKey:key];
}

/**
 * Очистка переменных окружения приложения
 * Вызываем нативный метод AppMetrica
 */
RCT_EXPORT_METHOD(clearAppEnvironment)
{
    [AMAAppMetrica clearAppEnvironment];
}

// MARK: - Внешняя атрибуция

/**
 * Отчет о внешней атрибуции
 * Вызываем нативный метод AppMetrica с внешней атрибуцией
 * 
 * @param attribution - словарь с внешней атрибуцией
 */
RCT_EXPORT_METHOD(reportExternalAttribution:(NSDictionary *)attribution)
{
    NSString *sourceStr = attribution[@"source"];
    AMAAttributionSource source = amarn_getExternalAttributionSource(sourceStr);
    if (source == nil) {
        NSLog(@"AppMetrica: Failed to report external attribution. Unknown source %@", sourceStr);
        return;
    }

    NSDictionary *value = attribution[@"value"];

    [AMAAppMetrica reportExternalAttribution:value source:source onFailure:^(NSError *error) {
        NSLog(@"Failed to report external attribution to AppMetrica: %@", [error localizedDescription]);
    }];
}

// MARK: - Startup параметры

/**
 * Запрос startup параметров
 * Вызываем нативный метод AppMetrica с callback
 * 
 * @param identifiers - массив идентификаторов
 * @param listener - callback для получения результата
 */
RCT_EXPORT_METHOD(requestStartupParams:(NSArray *)identifiers listener:(RCTResponseSenderBlock)listener)
{
    AMAIdentifiersCompletionBlock block = ^(NSDictionary<AMAStartupKey,id> * _Nullable identifiers, NSError * _Nullable error) {
        NSDictionary *result = [AMARNStartupParamsUtils toStrartupParamsResult:identifiers];
        NSString *errorStr = [AMARNStartupParamsUtils stringFromRequestStartupParamsError:error];
        listener(@[[self wrap:result], [self wrap:errorStr]]);
    };
    [AMAAppMetrica requestStartupIdentifiersWithKeys:[AMARNStartupParamsUtils toStartupKeys:identifiers] completionQueue:nil completionBlock:block];
}

// MARK: - Reporter методы

/**
 * Активация Reporter
 * Вызываем нативный метод AppMetrica с конфигурацией Reporter
 * 
 * @param config - словарь с конфигурацией Reporter
 */
RCT_EXPORT_METHOD(activateReporter:(NSDictionary *)config)
{
    [AMAAppMetrica activateReporterWithConfiguration:[AMARNAppMetricaUtils reporterConfigurationForDictionary:config]];
}

/**
 * Получение Reporter по API ключу
 * Вызываем нативный метод AppMetrica
 * 
 * @param apiKey - API ключ Reporter
 */
RCT_EXPORT_METHOD(touchReporter:(NSString *)apiKey)
{
    [AMAAppMetrica reporterForAPIKey:apiKey];
}

// MARK: - Получение идентификаторов

/**
 * Получение Device ID
 * Возвращаем Device ID устройства
 * 
 * @return NSString с Device ID
 */
RCT_EXPORT_METHOD(getDeviceId:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    resolve([AMAAppMetrica deviceID]);
}

/**
 * Получение UUID
 * Возвращаем UUID приложения
 * 
 * @return NSString с UUID
 */
RCT_EXPORT_METHOD(getUuid:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    resolve([AMAAppMetrica UUID]);
}

// MARK: - Deferred deeplink (только для Android, в iOS не реализовано)

/**
 * Запрос deferred deeplink (только для Android)
 * В iOS этот метод не реализован
 * 
 * @param onFailure - callback для ошибки
 * @param onSuccess - callback для успеха
 */
RCT_EXPORT_METHOD(requestDeferredDeeplink:(RCTResponseSenderBlock)onFailure onSuccess:(RCTResponseSenderBlock)onSuccess)
{
    // It does nothing for iOS
    onFailure(@[@"Deferred deeplink is not supported on iOS"]);
}

/**
 * Запрос параметров deferred deeplink (только для Android)
 * В iOS этот метод не реализован
 * 
 * @param onFailure - callback для ошибки
 * @param onSuccess - callback для успеха
 */
RCT_EXPORT_METHOD(requestDeferredDeeplinkParameters:(RCTResponseSenderBlock)onFailure onSuccess:(RCTResponseSenderBlock)onSuccess)
{
    // It does nothing for iOS
    onFailure(@[@"Deferred deeplink parameters are not supported on iOS"]);
}

// MARK: - Вспомогательные методы

/**
 * Обертка для nil значений
 * Конвертирует nil в NSNull для передачи в JavaScript
 * 
 * @param value - значение для обертки
 * @return NSObject с обернутым значением
 */
- (NSObject *)wrap:(NSObject *)value
{
    if (value == nil) {
        return [NSNull null];
    }
    return value;
}

@end