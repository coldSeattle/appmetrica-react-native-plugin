import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  activate(config: Object): void;
  getLibraryApiLevel(): Promise<number>;
  getLibraryVersion(): Promise<string>;
  pauseSession(): void;
  reportAppOpen(deeplink: string): void;
  reportError(identifier: string, message: string, reason: Object): void;
  reportEvent(eventName: string, attributes: Object | null): void;
  requestStartupParams(
    identifiers: string[],
    listener: (params: Object | null, reason: string | null) => void
  ): void;
  resumeSession(): void;
  sendEventsBuffer(): void;
  setLocation(location: Object | null): void;
  setLocationTracking(enabled: boolean): void;
  setDataSendingEnabled(enabled: boolean): void;
  reportECommerce(ecommerce: Object): void;
  setUserProfileID(userProfileID: string | null): void;
  reportRevenue(revenue: Object): void;
  reportAdRevenue(adRevenue: Object): void;
  reportUserProfile(userProfile: Object): void;
  putErrorEnvironmentValue(key: string, value: string | null): void;
  reportExternalAttribution(attribution: Object): void;
  reportErrorWithoutIdentifier(message: string, error: Object): void;
  reportUnhandledException(error: Object): void;
  putAppEnvironmentValue(key: string, value: string | null): void;
  clearAppEnvironment(): void;
  activateReporter(config: Object): void;
  touchReporter(apiKey: string): void;
  getDeviceId(): Promise<string | null>;
  getUuid(): Promise<string | null>;
  requestDeferredDeeplink(
    onFailure: () => void,
    onSuccess: (deeplink: string) => void
  ): void;
  requestDeferredDeeplinkParameters(
    onFailure: () => void,
    onSuccess: (params: Object) => void
  ): void;
}

export default TurboModuleRegistry.getEnforcing<Spec>('AppMetricaTurbo');
