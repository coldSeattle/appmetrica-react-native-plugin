// spec/AppMetricaSpec.ts
import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';
/**
 * TypeScript спецификация для AppMetrica TurboModule
 * Этот файл определяет интерфейс между JavaScript и нативным кодом
 * CodeGen автоматически генерирует C++ и Objective-C интерфейсы на основе этой спецификации
 */

export interface Spec extends TurboModule {
  // Основные методы активации и управления
  activate(config: Object): void;
  getLibraryApiLevel(): Promise<number>;
  getLibraryVersion(): Promise<string>;

  // Управление сессиями
  pauseSession(): void;
  resumeSession(): void;
  sendEventsBuffer(): void;

  // Отчеты о событиях и ошибках
  reportAppOpen(deeplink?: string): void;
  reportEvent(eventName: string, attributes?: Object): void;
  reportError(identifier: string, message?: string, reason?: Object): void;
  reportErrorWithoutIdentifier(message: string, error: Object): void;
  reportUnhandledException(error: Object): void;

  // E-commerce события
  reportECommerce(ecommerceEvent: Object): void;
  reportRevenue(revenue: Object): void;
  reportAdRevenue(adRevenue: Object): void;

  // Пользовательские данные
  reportUserProfile(userProfile: Object): void;
  setUserProfileID(userProfileID?: string): void;

  // Локация
  setLocation(location?: Object): void;
  setLocationTracking(enabled: boolean): void;

  // Настройки данных
  setDataSendingEnabled(enabled: boolean): void;
  putErrorEnvironmentValue(key: string, value?: string): void;
  putAppEnvironmentValue(key: string, value?: string): void;
  clearAppEnvironment(): void;

  // Внешняя атрибуция
  reportExternalAttribution(attribution: Object): void;

  // Startup параметры
  requestStartupParams(
    identifiers: string[],
    listener: (params?: Object, reason?: string) => void
  ): void;

  // Reporter методы
  activateReporter(config: Object): void;
  touchReporter(apiKey: string): void;

  // Получение идентификаторов
  getDeviceId(): Promise<string>;
  getUuid(): Promise<string>;

  // Deferred deeplink (только для Android, в iOS не реализовано)
  requestDeferredDeeplink(
    onFailure: (error: string) => void,
    onSuccess: (deeplink: string) => void
  ): void;
  requestDeferredDeeplinkParameters(
    onFailure: (error: string) => void,
    onSuccess: (params: Object) => void
  ): void;
}

/**
 * Получаем TurboModule из реестра
 * Если новая архитектура не включена, вернется null
 * В этом случае будем использовать старый Bridge
 */
export default TurboModuleRegistry.get<Spec>('AppMetrica') as Spec | null;
