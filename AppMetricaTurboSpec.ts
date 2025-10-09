import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

/**
 * AppMetricaTurboSpec - Полная спецификация для AppMetrica TurboModule
 *
 * Этот файл объединяет все методы AppMetrica в один интерфейс TurboModule
 * Обеспечивает полную совместимость с новой архитектурой React Native
 * CodeGen автоматически генерирует C++ и Objective-C интерфейсы на основе этой спецификации
 */
export interface Spec extends TurboModule {
  // ===== ОСНОВНЫЕ МЕТОДЫ АКТИВАЦИИ И УПРАВЛЕНИЯ =====

  /**
   * Активация AppMetrica с конфигурацией
   * @param config - объект конфигурации AppMetrica
   */
  activate(config: Object): void;

  /**
   * Получение API уровня библиотеки (только для Android)
   * @returns Promise с номером API уровня
   */
  getLibraryApiLevel(): Promise<number>;

  /**
   * Получение версии библиотеки
   * @returns Promise с версией библиотеки
   */
  getLibraryVersion(): Promise<string>;

  // ===== УПРАВЛЕНИЕ СЕССИЯМИ =====

  /**
   * Приостановка текущей сессии
   */
  pauseSession(): void;

  /**
   * Возобновление сессии
   */
  resumeSession(): void;

  /**
   * Отправка буфера событий
   */
  sendEventsBuffer(): void;

  // ===== ОТЧЕТЫ О СОБЫТИЯХ И ОШИБКАХ =====

  /**
   * Отчет об открытии приложения
   * @param deeplink - опциональная deeplink строка
   */
  reportAppOpen(deeplink?: string): void;

  /**
   * Отчет о пользовательском событии
   * @param eventName - название события
   * @param attributes - опциональные атрибуты события
   */
  reportEvent(eventName: string, attributes?: Object): void;

  /**
   * Отчет об ошибке с идентификатором
   * @param identifier - идентификатор ошибки
   * @param message - опциональное сообщение об ошибке
   * @param reason - опциональный объект с причиной ошибки
   */
  reportError(identifier: string, message?: string, reason?: Object): void;

  /**
   * Отчет об ошибке без идентификатора
   * @param message - сообщение об ошибке
   * @param error - объект ошибки
   */
  reportErrorWithoutIdentifier(message: string, error: Object): void;

  /**
   * Отчет о необработанном исключении
   * @param error - объект исключения
   */
  reportUnhandledException(error: Object): void;

  // ===== E-COMMERCE СОБЫТИЯ =====

  /**
   * Отчет о e-commerce событии
   * @param ecommerceEvent - объект e-commerce события
   */
  reportECommerce(ecommerceEvent: Object): void;

  /**
   * Отчет о доходе
   * @param revenue - объект дохода
   */
  reportRevenue(revenue: Object): void;

  /**
   * Отчет о доходе от рекламы
   * @param adRevenue - объект дохода от рекламы
   */
  reportAdRevenue(adRevenue: Object): void;

  // ===== ПОЛЬЗОВАТЕЛЬСКИЕ ДАННЫЕ =====

  /**
   * Отчет о профиле пользователя
   * @param userProfile - объект профиля пользователя
   */
  reportUserProfile(userProfile: Object): void;

  /**
   * Установка ID профиля пользователя
   * @param userProfileID - опциональный ID профиля пользователя
   */
  setUserProfileID(userProfileID?: string): void;

  // ===== ЛОКАЦИЯ =====

  /**
   * Установка локации
   * @param location - опциональный объект локации
   */
  setLocation(location?: Object): void;

  /**
   * Включение/выключение отслеживания локации
   * @param enabled - включить отслеживание локации
   */
  setLocationTracking(enabled: boolean): void;

  // ===== НАСТРОЙКИ ДАННЫХ =====

  /**
   * Включение/выключение отправки данных
   * @param enabled - включить отправку данных
   */
  setDataSendingEnabled(enabled: boolean): void;

  /**
   * Добавление значения в окружение ошибок
   * @param key - ключ
   * @param value - опциональное значение
   */
  putErrorEnvironmentValue(key: string, value?: string): void;

  /**
   * Добавление значения в окружение приложения
   * @param key - ключ
   * @param value - опциональное значение
   */
  putAppEnvironmentValue(key: string, value?: string): void;

  /**
   * Очистка окружения приложения
   */
  clearAppEnvironment(): void;

  // ===== ВНЕШНЯЯ АТРИБУЦИЯ =====

  /**
   * Отчет о внешней атрибуции
   * @param attribution - объект атрибуции
   */
  reportExternalAttribution(attribution: Object): void;

  // ===== STARTUP ПАРАМЕТРЫ =====

  /**
   * Запрос startup параметров
   * @param identifiers - массив идентификаторов
   * @param listener - функция обратного вызова для получения параметров
   */
  requestStartupParams(
    identifiers: string[],
    listener: (params?: Object, reason?: string) => void
  ): void;

  // ===== REPORTER МЕТОДЫ =====

  /**
   * Активация репортера
   * @param config - конфигурация репортера
   */
  activateReporter(config: Object): void;

  /**
   * Активация репортера по API ключу
   * @param apiKey - API ключ репортера
   */
  touchReporter(apiKey: string): void;

  // ===== ПОЛУЧЕНИЕ ИДЕНТИФИКАТОРОВ =====

  /**
   * Получение ID устройства
   * @returns Promise с ID устройства
   */
  getDeviceId(): Promise<string>;

  /**
   * Получение UUID
   * @returns Promise с UUID
   */
  getUuid(): Promise<string>;

  // ===== DEFERRED DEEPLINK (только для Android) =====

  /**
   * Запрос deferred deeplink
   * @param onFailure - функция обратного вызова при ошибке
   * @param onSuccess - функция обратного вызова при успехе
   */
  requestDeferredDeeplink(
    onFailure: (error: string) => void,
    onSuccess: (deeplink: string) => void
  ): void;

  /**
   * Запрос параметров deferred deeplink
   * @param onFailure - функция обратного вызова при ошибке
   * @param onSuccess - функция обратного вызова при успехе
   */
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
export default TurboModuleRegistry.get<Spec>('AppMetricaTurbo') as Spec | null;
