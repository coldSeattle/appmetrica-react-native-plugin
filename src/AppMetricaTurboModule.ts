import { TurboModuleRegistry } from 'react-native';
import type { Spec } from './AppMetricaTurboSpec';

const AppMetricaTurbo =
  TurboModuleRegistry.getEnforcing<Spec>('AppMetricaTurbo');

export default AppMetricaTurbo;
