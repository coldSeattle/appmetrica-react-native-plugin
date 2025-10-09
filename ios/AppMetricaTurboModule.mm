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

RCT_EXPORT_MODULE(AppMetrica)

// MARK: - Основные методы активации и управления

/**
 * Активация AppMetrica с конфигурацией
 * Вызывается напрямую из JavaScript через JSI
 * 
 * @param config - словарь с конфигурацией AppMetrica
 */
- (void)activate:(NSDictionary *)config {
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
- (NSNumber *)getLibraryApiLevel {
    return @0; // iOS не поддерживает API level
}

/**
 * Получение версии библиотеки
 * Возвращаем версию библиотеки AppMetrica
 * 
 * @return NSString с версией библиотеки
 */
- (NSString *)getLibraryVersion {
    return [AMAAppMetrica libraryVersion];
}

// MARK: - Управление сессиями

/**
 * Приостановка сессии
 * Вызываем нативный метод AppMetrica
 */
- (void)pauseSession {
    [AMAAppMetrica pauseSession];
}

/**
 * Возобновление сессии
 * Вызываем нативный метод AppMetrica
 */
- (void)resumeSession {
    [AMAAppMetrica resumeSession];
}

/**
 * Отправка буфера событий
 * Вызываем нативный метод AppMetrica
 */
- (void)sendEventsBuffer {
    [AMAAppMetrica sendEventsBuffer];
}

// MARK: - Отчеты о событиях и ошибках

/**
 * Отчет об открытии приложения
 * Конвертируем deeplink в URL и вызываем нативный метод
 * 
 * @param deeplink - строка с deeplink (может быть nil)
 */
- (void)reportAppOpen:(NSString *)deeplink {
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
- (void)reportEvent:(NSString *)eventName attributes:(NSDictionary *)attributes {
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
- (void)reportError:(NSString *)identifier message:(NSString *)message reason:(NSDictionary *)reason {
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
- (void)reportErrorWithoutIdentifier:(NSString *)message error:(NSDictionary *)error {
    AMAPluginErrorDetails *details = amarn_exceptionForDictionary(error);
    if (details.backtrace.count == 0) {
        [[[AMAAppMetricaCrashes crashes] pluginExtension] reportErrorWithIdentifier:@"Errors without stacktrace"
                                                                            message:message
                                                                            details:details
                                                                          onFailure:^(NSError *error) {
            NSLog(@"Failed to report error to AppMetrica: %@", [error localizedDescription]);
        }];
    } else {
        [[[AMAAppMetricaCrashes crashes] pluginExtension] reportError:details message:message onFailure:^(NSError *error) {
            NSLog(@"Failed to report error to AppMetrica: %@", [error localizedDescription]);
        }];
    }
}

/**
 * Отчет о необработанном исключении
 * Вызываем нативный метод AppMetrica с деталями ошибки
 * 
 * @param error - словарь с деталями ошибки
 */
- (void)reportUnhandledException:(NSDictionary *)error {
    [[[AMAAppMetricaCrashes crashes] pluginExtension] reportUnhandledException:amarn_exceptionForDictionary(error)
                                                                     onFailure:^(NSError *error) {
        NSLog(@"Failed to report unhandled exception to AppMetrica: %@", [error localizedDescription]);
    }];
}

// MARK: - E-commerce события

/**
 * Отчет о E-commerce событии
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param ecommerceEvent - словарь с данными E-commerce события
 */
- (void)reportECommerce:(NSDictionary *)ecommerceEvent {
    [AMAAppMetrica reportECommerce:[AMARNAppMetricaUtils ecommerceForDict:ecommerceEvent] onFailure:nil];
}

/**
 * Отчет о доходе
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param revenue - словарь с данными о доходе
 */
- (void)reportRevenue:(NSDictionary *)revenue {
    [AMAAppMetrica reportRevenue:[AMARNAppMetricaUtils revenueForDict:revenue] onFailure:nil];
}

/**
 * Отчет о доходе от рекламы
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param adRevenue - словарь с данными о доходе от рекламы
 */
- (void)reportAdRevenue:(NSDictionary *)adRevenue {
    [AMAAppMetrica reportAdRevenue:[AMARNAppMetricaUtils adRevenueForDict:adRevenue] onFailure:nil];
}

// MARK: - Пользовательские данные

/**
 * Отчет о профиле пользователя
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param userProfile - словарь с данными профиля пользователя
 */
- (void)reportUserProfile:(NSDictionary *)userProfile {
    [AMAAppMetrica reportUserProfile:[AMARNAppMetricaUtils userProfileForDict:userProfile] onFailure:nil];
}

/**
 * Установка ID профиля пользователя
 * Вызываем нативный метод AppMetrica
 * 
 * @param userProfileID - ID профиля пользователя (может быть nil)
 */
- (void)setUserProfileID:(NSString *)userProfileID {
    [AMAAppMetrica setUserProfileID:userProfileID];
}

// MARK: - Локация

/**
 * Установка локации
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param location - словарь с данными локации (может быть nil)
 */
- (void)setLocation:(NSDictionary *)location {
    AMAAppMetrica.customLocation = [AMARNAppMetricaUtils locationForDictionary:location];
}

/**
 * Включение/выключение отслеживания локации
 * Вызываем нативный метод AppMetrica
 * 
 * @param enabled - включить или выключить отслеживание локации
 */
- (void)setLocationTracking:(BOOL)enabled {
    AMAAppMetrica.locationTrackingEnabled = enabled;
}

// MARK: - Настройки данных

/**
 * Включение/выключение отправки данных
 * Вызываем нативный метод AppMetrica
 * 
 * @param enabled - включить или выключить отправку данных
 */
- (void)setDataSendingEnabled:(BOOL)enabled {
    [AMAAppMetrica setDataSendingEnabled:enabled];
}

/**
 * Установка значения окружения ошибки
 * Вызываем нативный метод AppMetrica
 * 
 * @param key - ключ
 * @param value - значение (может быть nil)
 */
- (void)putErrorEnvironmentValue:(NSString *)key value:(NSString *)value {
    [[AMAAppMetricaCrashes crashes] setErrorEnvironmentValue:value forKey:key];
}

/**
 * Установка значения окружения приложения
 * Вызываем нативный метод AppMetrica
 * 
 * @param key - ключ
 * @param value - значение (может быть nil)
 */
- (void)putAppEnvironmentValue:(NSString *)key value:(NSString *)value {
    [AMAAppMetrica setAppEnvironmentValue:value forKey:key];
}

/**
 * Очистка окружения приложения
 * Вызываем нативный метод AppMetrica
 */
- (void)clearAppEnvironment {
    [AMAAppMetrica clearAppEnvironment];
}

// MARK: - Внешняя атрибуция

/**
 * Отчет о внешней атрибуции
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param attribution - словарь с данными внешней атрибуции
 */
- (void)reportExternalAttribution:(NSDictionary *)attribution {
    NSString *sourceStr = attribution[@"source"];
    AMAAttributionSource source = amarn_getExternalAttributionSource(sourceStr);
    if (source == nil) {
        NSLog(@"Failed to report external attribution to AppMetrica. Unknown source %@", sourceStr);
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
 * Создаем callback и вызываем нативный метод
 * 
 * @param identifiers - массив идентификаторов
 * @param listener - callback функция для получения результата
 */
- (void)requestStartupParams:(NSArray *)identifiers listener:(RCTResponseSenderBlock)listener {
    AMAIdentifiersCompletionBlock block = ^(NSDictionary<AMAStartupKey,id> * _Nullable identifiers, NSError * _Nullable error) {
        NSDictionary *result = [AMARNStartupParamsUtils toStrartupParamsResult:identifiers];
        NSString *errorStr = [AMARNStartupParamsUtils stringFromRequestStartupParamsError:error];
        listener(@[[self wrap:result], [self wrap:errorStr]]);
    };
    [AMAAppMetrica requestStartupIdentifiersWithKeys:[AMARNStartupParamsUtils toStartupKeys:identifiers] completionQueue:nil completionBlock:block];
}

// MARK: - Reporter методы

/**
 * Активация reporter
 * Конвертируем параметры и вызываем нативный метод
 * 
 * @param config - словарь с конфигурацией reporter
 */
- (void)activateReporter:(NSDictionary *)config {
    [AMAAppMetrica activateReporterWithConfiguration:[AMARNAppMetricaUtils reporterConfigurationForDictionary:config]];
}

/**
 * Создание reporter
 * Вызываем нативный метод AppMetrica
 * 
 * @param apiKey - API ключ для reporter
 */
- (void)touchReporter:(NSString *)apiKey {
    [AMAAppMetrica reporterForAPIKey:apiKey];
}

// MARK: - Получение идентификаторов

/**
 * Получение Device ID
 * Возвращаем Device ID приложения
 * 
 * @return NSString с Device ID
 */
- (NSString *)getDeviceId {
    return [AMAAppMetrica deviceID];
}

/**
 * Получение UUID
 * Возвращаем UUID приложения
 * 
 * @return NSString с UUID
 */
- (NSString *)getUuid {
    return [AMAAppMetrica UUID];
}

// MARK: - Deferred deeplink (только для Android)

/**
 * Запрос deferred deeplink (не реализовано в iOS)
 * Возвращаем пустую функцию
 * 
 * @param onFailure - callback для ошибки
 * @param onSuccess - callback для успеха
 */
- (void)requestDeferredDeeplink:(RCTResponseSenderBlock)onFailure onSuccess:(RCTResponseSenderBlock)onSuccess {
    // iOS не поддерживает deferred deeplink
}

/**
 * Запрос параметров deferred deeplink (не реализовано в iOS)
 * Возвращаем пустую функцию
 * 
 * @param onFailure - callback для ошибки
 * @param onSuccess - callback для успеха
 */
- (void)requestDeferredDeeplinkParameters:(RCTResponseSenderBlock)onFailure onSuccess:(RCTResponseSenderBlock)onSuccess {
    // iOS не поддерживает deferred deeplink
}

// MARK: - Вспомогательные методы

/**
 * Вспомогательный метод для обертывания nil значений
 * Конвертирует nil в NSNull для корректной передачи в JavaScript
 * 
 * @param value - значение для обертывания
 * @return NSObject - обернутое значение
 */
- (NSObject *)wrap:(NSObject *)value {
    if (value == nil) {
        return [NSNull null];
    }
    return value;
}

@end
