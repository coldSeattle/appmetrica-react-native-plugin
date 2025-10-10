import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

// Basic types
export interface Location {
  latitude: number;
  longitude: number;
  altitude?: number;
  accuracy?: number;
  course?: number;
  speed?: number;
  timestamp?: number;
}

export interface PreloadInfo {
  trackingId: string;
  additionalInfo?: Object;
}

export interface AppMetricaConfig {
  apiKey: string;
  appVersion?: string;
  crashReporting?: boolean;
  firstActivationAsUpdate?: boolean;
  location?: Location;
  locationTracking?: boolean;
  logs?: boolean;
  sessionTimeout?: number;
  statisticsSending?: boolean;
  preloadInfo?: PreloadInfo;
  maxReportsInDatabaseCount?: number;
  nativeCrashReporting?: boolean; // Android only
  activationAsSessionStart?: boolean; // iOS only
  sessionsAutoTracking?: boolean; // iOS only
  appOpenTrackingEnabled?: boolean;
  userProfileID?: string;
  errorEnvironment?: Object;
  appEnvironment?: Object;
  maxReportsCount?: number;
  dispatchPeriodSeconds?: number;
}

// ECommerce types
export interface ECommerceAmount {
  amount: string;
  unit: string;
}

export interface ECommercePrice {
  amount: ECommerceAmount;
  internalComponents?: Array<ECommerceAmount>;
}

export interface ECommerceProduct {
  sku: string;
  name?: string;
  actualPrice?: ECommercePrice;
  originalPrice?: ECommercePrice;
  promocodes?: Array<string>;
  categoriesPath?: Array<string>;
  payload?: Object;
}

export interface ECommerceScreen {
  name: string;
  searchQuery?: string;
  payload?: Object;
  categoriesPath?: Array<string>;
}

export interface ECommerceReferrer {
  type?: string;
  identifier?: string;
  screen?: ECommerceScreen;
}

export interface ECommerceCartItem {
  product: ECommerceProduct;
  price: ECommercePrice;
  quantity: string;
  referrer?: ECommerceReferrer;
}

export interface ECommerceOrder {
  orderId: string;
  products: Array<ECommerceCartItem>;
  payload?: Object;
}

export type ECommerceEventType = string;

export interface ECommerceEvent {
  ecommerceEvent: ECommerceEventType;
  ecommerceScreen?: ECommerceScreen;
  product?: ECommerceProduct;
  referrer?: ECommerceReferrer;
  cartItem?: ECommerceCartItem;
  order?: ECommerceOrder;
}

// Revenue types
export interface Receipt {
  transactionID?: string;
  receiptData?: string;
  signature?: string;
}

export interface Revenue {
  price: number;
  currency: string;
  productID?: string;
  quantity?: number;
  payload?: string;
  receipt?: Receipt;
}

export type AdType = string;

export interface AdRevenue {
  price: string;
  currency: string;
  payload?: Object;
  adNetwork?: string;
  adPlacementID?: string;
  adPlacementName?: string;
  adType?: AdType;
  adUnitID?: string;
  adUnitName?: string;
  precision?: string;
}

// UserProfile types
export type UserProfileUpdateType = string;

export interface UserProfileUpdate {
  type: UserProfileUpdateType;
  key?: string;
  value?: Object;
  ifUndefined?: boolean;
  age?: number;
  year?: number;
  month?: number;
  day?: number;
  delta?: number;
}

export interface UserProfile {
  attributes: Array<UserProfileUpdate>;
}

// External Attribution types
export type ExternalAttributionSource = string;

export interface ExternalAttribution {
  source: ExternalAttributionSource;
  value: Object;
}

// Error types
export interface AppMetricaError {
  identifier?: string;
  message?: string;
  stacktrace?: Array<string>;
  platform?: string;
  virtualMachineVersion?: string;
  buildId?: string;
  buildType?: string;
  multidex?: boolean;
  threads?: Array<AppMetricaThread>;
}

export interface AppMetricaThread {
  name?: string;
  state?: string;
  crashed?: boolean;
  stacktrace?: Array<string>;
}

// Startup Params types
export type StartupParamsReason = string;

export interface StartupParams {
  deviceIdHash?: string;
  deviceId?: string;
  uuid?: string;
}

// Deferred Deeplink types
export type DeferredDeeplinkError = string;

// Reporter Config
export interface ReporterConfig {
  apiKey: string;
  logs?: boolean;
  maxReportsInDatabaseCount?: number;
  sessionTimeout?: number;
  dataSendingEnabled?: boolean;
  appEnvironment?: Object;
  dispatchPeriodSeconds?: number;
  userProfileID?: string;
  maxReportsCount?: number;
}

// Main AppMetrica Spec
export interface Spec extends TurboModule {
  // Configuration
  activate(config: AppMetricaConfig): void;

  // Library info
  getLibraryApiLevel(): Promise<number>;
  getLibraryVersion(): Promise<string>;

  // Session management
  pauseSession(): void;
  resumeSession(): void;
  sendEventsBuffer(): void;

  // App tracking
  reportAppOpen(deeplink?: string): void;

  // Error reporting
  reportError(
    identifier: string,
    message?: string,
    reason?: AppMetricaError
  ): void;
  reportUnhandledException(error: AppMetricaError): void;
  reportErrorWithoutIdentifier(message: string, error: AppMetricaError): void;

  // Event reporting
  reportEvent(eventName: string, attributes?: Object): void;

  // Location
  setLocation(location?: Location): void;
  setLocationTracking(enabled: boolean): void;

  // Data sending
  setDataSendingEnabled(enabled: boolean): void;

  // User profile
  setUserProfileID(userProfileID?: string): void;
  reportUserProfile(userProfile: UserProfile): void;

  // ECommerce
  reportECommerce(event: ECommerceEvent): void;

  // Revenue
  reportRevenue(revenue: Revenue): void;
  reportAdRevenue(adRevenue: AdRevenue): void;

  // Environment
  putErrorEnvironmentValue(key: string, value?: string): void;
  putAppEnvironmentValue(key: string, value?: string): void;
  clearAppEnvironment(): void;

  // External Attribution
  reportExternalAttribution(attribution: ExternalAttribution): void;

  // Reporter management
  activateReporter(config: ReporterConfig): void;
  touchReporter(apiKey: string): void;

  // Device info
  getDeviceId(): Promise<string>;
  getUuid(): Promise<string>;

  // Startup params
  requestStartupParams(
    identifiers: Array<string>,
    callback: (params?: StartupParams, reason?: StartupParamsReason) => void
  ): void;

  // Deferred deeplinks
  requestDeferredDeeplink(
    onFailure: (error: DeferredDeeplinkError, referrer?: string) => void,
    onSuccess: (deeplink: string) => void
  ): void;

  requestDeferredDeeplinkParameters(
    onFailure: (error: DeferredDeeplinkError, referrer?: string) => void,
    onSuccess: (parameters: Object) => void
  ): void;
}

export default TurboModuleRegistry.getEnforcing<Spec>('NativeAppMetrica');
