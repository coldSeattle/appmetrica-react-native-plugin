# 📱 AppMetrica React Native - Установка для iOS с новой архитектурой

## 🚀 **Быстрая установка**

### **Шаг 1: Установка библиотеки**

```bash
npm install @appmetrica/react-native-analytics
# или
yarn add @appmetrica/react-native-analytics
```

### **Шаг 2: Настройка iOS для новой архитектуры**

#### **2.1. Обновите `ios/Podfile`:**

```ruby
# ios/Podfile

# Включение новой архитектуры
ENV['RCT_NEW_ARCH_ENABLED'] = '1'

# Использование статических фреймворков
use_frameworks! :linkage => :static

# Подключение React Native
require_relative '../node_modules/react-native/scripts/react_native_pods'
require_relative '../node_modules/@react-native-community/cli-platform-ios/native_modules'

platform :ios, min_ios_version_supported
prepare_react_native_project!

# Включение новой архитектуры
use_react_native!(
  :path => config[:reactNativePath],
  :hermes_enabled => true,
  :fabric_enabled => true,  # ← Включаем Fabric
  :new_arch_enabled => true # ← Включаем новую архитектуру
)

target 'YourApp' do
  config = use_native_modules!

  # Подключение AppMetrica
  pod 'appmetrica-react-native-analytics', :path => '../node_modules/@appmetrica/react-native-analytics'

  # Остальные поды...
end
```

#### **2.2. Установка подов:**

```bash
cd ios
pod install
```

#### **2.3. Очистка и пересборка:**

```bash
# В Xcode: Product → Clean Build Folder
# Затем: Product → Build
```

### **Шаг 3: Использование в коде**

#### **3.1. Импорт и активация:**

```typescript
// App.tsx или index.js
import AppMetrica from '@appmetrica/react-native-analytics';

// Активация AppMetrica
AppMetrica.activate({
  apiKey: 'YOUR_API_KEY',
  appVersion: '1.0.0',
  crashReporting: true,
  locationTracking: true,
});
```

#### **3.2. Отправка событий:**

```typescript
// Простые события
AppMetrica.reportEvent('user_login', {
  method: 'email',
  timestamp: Date.now(),
});

// E-commerce события
AppMetrica.reportECommerce({
  ecommerceEvent: 'purchaseEvent',
  order: {
    orderId: '12345',
    products: [
      {
        product: {
          sku: 'product_sku',
          name: 'Product Name',
          actualPrice: {
            amount: '100.00',
            unit: 'RUB',
          },
        },
        quantity: 1,
        price: {
          amount: '100.00',
          unit: 'RUB',
        },
      },
    ],
  },
});

// Ошибки
AppMetrica.reportError('custom_error', 'Something went wrong', {
  stacktrace: 'Error stacktrace...',
  platform: 'ios',
});
```

## 🔧 **Проверка работы новой архитектуры**

### **Тест 1: Проверка доступности TurboModule**

```typescript
import NativeAppMetrica from '@appmetrica/react-native-analytics/spec/AppMetricaSpec';

// Если новая архитектура включена, это будет объект
// Если старая архитектура, это будет null
console.log('TurboModule доступен:', !!NativeAppMetrica);
```

### **Тест 2: Проверка производительности**

```typescript
// Тест производительности
const startTime = Date.now();
for (let i = 0; i < 1000; i++) {
  AppMetrica.reportEvent('test_event', { iteration: i });
}
const endTime = Date.now();
console.log(`1000 событий за ${endTime - startTime}ms`);
```

## 🐛 **Решение проблем**

### **Проблема 1: Ошибка компиляции C++**

```bash
# Решение: Очистите кэш
cd ios
rm -rf build
pod deintegrate
pod install
```

### **Проблема 2: TurboModule не найден**

```bash
# Решение: Проверьте переменную окружения
echo $RCT_NEW_ARCH_ENABLED
# Должно быть: 1
```

### **Проблема 3: Ошибки линковки**

```bash
# Решение: Обновите Xcode и пересоберите
# Xcode → Product → Clean Build Folder
# Xcode → Product → Build
```

## 📊 **Преимущества новой архитектуры**

| Аспект                 | Старая архитектура | Новая архитектура |
| ---------------------- | ------------------ | ----------------- |
| **Производительность** | ~1-5ms             | ~0.1ms            |
| **Типобезопасность**   | ❌                 | ✅                |
| **Синхронность**       | ❌                 | ✅                |
| **Память**             | Больше             | Меньше            |
| **Отладка**            | Сложнее            | Проще             |

## 🎯 **Готово!**

Ваша библиотека AppMetrica теперь работает на новой архитектуре React Native для iOS! 🚀

**Основные преимущества:**

- ✅ **Высокая производительность** - прямые вызовы через JSI
- ✅ **Типобезопасность** - TypeScript интерфейсы
- ✅ **Совместимость** - автоматический fallback на старую архитектуру
- ✅ **Простота использования** - тот же API, что и раньше
