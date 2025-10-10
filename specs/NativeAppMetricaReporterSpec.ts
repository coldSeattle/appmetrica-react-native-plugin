import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

// Import types from main spec
export interface ECommerceEvent {
  ecommerceEvent: string;
  ecommerceScreen?: Object;
  product?: Object;
  referrer?: Object;
  cartItem?: Object;
  order?: Object;
}

export interface Revenue {
  price: number;
  currency: string;
  productID?: string;
  quantity?: number;
  payload?: string;
  receipt?: Object;
}

export interface AdRevenue {
  price: string;
  currency: string;
  payload?: Object;
  adNetwork?: string;
  adPlacementID?: string;
  adPlacementName?: string;
  adType?: string;
  adUnitID?: string;
  adUnitName?: string;
  precision?: string;
}

export interface UserProfile {
  attributes: Array<Object>;
}

export interface AppMetricaError {
  identifier?: string;
  message?: string;
  stacktrace?: Array<string>;
  platform?: string;
  virtualMachineVersion?: string;
  buildId?: string;
  buildType?: string;
  multidex?: boolean;
  threads?: Array<Object>;
}

// Reporter Spec
export interface Spec extends TurboModule {
  // Error reporting
  reportError(
    apiKey: string,
    identifier: string,
    message?: string,
    reason?: AppMetricaError
  ): void;

  reportErrorWithoutIdentifier(
    apiKey: string,
    message: string,
    error: AppMetricaError
  ): void;

  reportUnhandledException(apiKey: string, error: AppMetricaError): void;

  // Event reporting
  reportEvent(apiKey: string, eventName: string, attributes?: Object): void;

  // Session management
  pauseSession(apiKey: string): void;
  resumeSession(apiKey: string): void;
  sendEventsBuffer(apiKey: string): void;

  // Environment
  clearAppEnvironment(apiKey: string): void;
  putAppEnvironmentValue(apiKey: string, key: string, value?: string): void;

  // User profile
  setUserProfileID(apiKey: string, userProfileID: string): void;
  setDataSendingEnabled(apiKey: string, enabled: boolean): void;
  reportUserProfile(apiKey: string, userProfile: UserProfile): void;

  // Revenue reporting
  reportAdRevenue(apiKey: string, adRevenue: AdRevenue): void;
  reportECommerce(apiKey: string, event: ECommerceEvent): void;
  reportRevenue(apiKey: string, revenue: Revenue): void;
}

export default TurboModuleRegistry.getEnforcing<Spec>(
  'NativeAppMetricaReporter'
);
