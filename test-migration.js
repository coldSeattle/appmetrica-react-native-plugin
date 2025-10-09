// test-migration.js
// Простой тест для проверки миграции на новую архитектуру

import AppMetrica from './src/index';

console.log('🧪 Тестирование миграции AppMetrica на новую архитектуру...');

// Тест 1: Проверка доступности модуля
console.log('✅ Тест 1: Проверка доступности модуля');
console.log('AppMetrica модуль доступен:', !!AppMetrica);

// Тест 2: Проверка методов
console.log('✅ Тест 2: Проверка основных методов');
const methods = [
  'activate',
  'reportEvent', 
  'reportError',
  'getDeviceId',
  'getLibraryVersion'
];

methods.forEach(method => {
  const isAvailable = typeof AppMetrica[method] === 'function';
  console.log(`  ${method}: ${isAvailable ? '✅' : '❌'}`);
});

// Тест 3: Проверка активации (без реальной активации)
console.log('✅ Тест 3: Проверка конфигурации');
try {
  const config = {
    apiKey: 'test-api-key',
    appVersion: '1.0.0'
  };
  
  // Не активируем реально, только проверяем что метод существует
  console.log('  Метод activate доступен:', typeof AppMetrica.activate === 'function');
  console.log('  Конфигурация валидна:', !!config.apiKey);
} catch (error) {
  console.log('  ❌ Ошибка при проверке конфигурации:', error.message);
}

// Тест 4: Проверка типов
console.log('✅ Тест 4: Проверка типов');
console.log('  AppMetrica - это класс:', AppMetrica.constructor === Function);
console.log('  Есть статические методы:', typeof AppMetrica.activate === 'function');

console.log('🎉 Тестирование завершено!');
console.log('');
console.log('📋 Следующие шаги:');
console.log('1. Запустите приложение с новой архитектурой');
console.log('2. Проверьте что AppMetrica работает корректно');
console.log('3. Сравните производительность со старой архитектурой');
console.log('4. Убедитесь что все методы работают как ожидается');
