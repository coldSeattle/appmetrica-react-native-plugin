# 🧪 Чек-лист для тестирования новой архитектуры

## ✅ Готово к тестированию:

### 1. Файлы созданы:
- ✅ `src/AppMetricaTurboSpec.ts` - спек-файл Turbo Module
- ✅ `src/AppMetricaTurboModule.ts` - TypeScript wrapper
- ✅ `ios/AppMetricaTurboModule.h` - iOS header
- ✅ `ios/AppMetricaTurboModule.mm` - iOS implementation
- ✅ `package.json` - обновлен с `codegenConfig`
- ✅ `appmetrica-react-native-analytics.podspec` - обновлен для новой архитектуры

### 2. TypeScript компиляция:
- ✅ Без ошибок

## 🚀 Шаги для тестирования в вашем проекте:

### Шаг 1: Обновите Podfile в вашем проекте
```ruby
# В ios/Podfile добавьте:
use_frameworks! :linkage => :static
ENV['RCT_NEW_ARCH_ENABLED'] = '1'
```

### Шаг 2: Установите зависимости
```bash
cd ios && pod install
```

### Шаг 3: Очистите проект
```bash
cd ios && xcodebuild clean
# Или через Xcode: Product → Clean Build Folder
```

### Шаг 4: Пересоберите проект
```bash
npx react-native run-ios
```

## 🧪 Тесты для проверки:

### Тест 1: Проверка инициализации
```typescript
import AppMetrica from '@appmetrica/react-native-analytics';

// Должно работать без ошибок
AppMetrica.activate({
  apiKey: 'YOUR_API_KEY',
  logs: true
});
```

### Тест 2: Проверка событий
```typescript
// Должно работать без ошибок
AppMetrica.reportEvent('test_turbo_module', {
  architecture: 'new',
  timestamp: Date.now()
});
```

### Тест 3: Проверка версии библиотеки
```typescript
// Должно вернуть версию AppMetrica
const version = await AppMetrica.getLibraryVersion();
console.log('AppMetrica version:', version);
```

## 🔍 Что проверить:

1. **Консоль Xcode** - нет ошибок компиляции
2. **Metro bundler** - нет ошибок JavaScript
3. **Приложение запускается** без крашей
4. **События отправляются** в AppMetrica
5. **Fallback работает** если новая архитектура недоступна

## ⚠️ Возможные проблемы:

### Если Turbo Module не найден:
- Проверьте `ENV['RCT_NEW_ARCH_ENABLED'] = '1'` в Podfile
- Убедитесь что React Native 0.68+
- Перезапустите Metro bundler

### Если есть ошибки компиляции:
- Очистите проект: `cd ios && xcodebuild clean`
- Удалите папку `ios/build`
- Переустановите pods: `cd ios && pod install`

### Если приложение крашится:
- Проверьте логи в Xcode Console
- Убедитесь что все зависимости установлены
- Проверьте совместимость версий

## 📊 Ожидаемый результат:

- ✅ Приложение запускается
- ✅ AppMetrica инициализируется
- ✅ События отправляются
- ✅ Нет ошибок в консоли
- ✅ Лучшая производительность (если новая архитектура активна)

## 🎯 Готово к тестированию!

Все файлы созданы и настроены правильно. Можете приступать к тестированию в вашем проекте!
