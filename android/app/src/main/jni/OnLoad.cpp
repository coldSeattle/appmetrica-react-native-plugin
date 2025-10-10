#include <DefaultComponentsRegistry.h>
#include <DefaultTurboModuleManagerDelegate.h>
#include <autolinking.h>
#include <fbjni/fbjni.h>
#include <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>
#include <rncore.h>

// Include the AppMetrica module headers
#include <NativeAppMetrica.h>
#include <NativeAppMetricaReporter.h>

using namespace facebook;

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    return facebook::jni::initialize(vm, []
                                     {
    facebook::react::DefaultComponentsRegistry::registerComponentDescriptorsFromEntryPoint();
    facebook::react::DefaultTurboModuleManagerDelegate::registerComponentsFromEntryPoint(); });
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *)
{
    facebook::jni::unload();
}

extern "C" JNIEXPORT void JNICALL
Java_com_facebook_react_ReactPackage_initializeSoLoaderIfNecessary(JNIEnv *env, jclass)
{
    facebook::soloader::SoLoader::init(env, nullptr);
}

extern "C" JNIEXPORT void JNICALL
Java_com_facebook_react_ReactPackage_initializeFlipper(JNIEnv *env, jclass)
{
    // Flipper initialization can be added here if needed
}

extern "C" JNIEXPORT void JNICALL
Java_com_facebook_react_ReactPackage_initializeFabric(JNIEnv *env, jclass)
{
    facebook::react::DefaultComponentsRegistry::registerComponentDescriptorsFromEntryPoint();
    facebook::react::DefaultTurboModuleManagerDelegate::registerComponentsFromEntryPoint();
}

extern "C" JNIEXPORT void JNICALL
Java_com_facebook_react_ReactPackage_initializeTurboModules(JNIEnv *env, jclass)
{
    facebook::react::DefaultTurboModuleManagerDelegate::registerComponentsFromEntryPoint();
}

std::shared_ptr<facebook::react::TurboModule> cxxModuleProvider(
    const std::string &name,
    const std::shared_ptr<facebook::react::CallInvoker> &jsInvoker)
{
    // Register AppMetrica modules
    if (name == "NativeAppMetrica")
    {
        return std::make_shared<facebook::react::NativeAppMetrica>(jsInvoker);
    }

    if (name == "NativeAppMetricaReporter")
    {
        return std::make_shared<facebook::react::NativeAppMetricaReporter>(jsInvoker);
    }

    // And we fallback to the CXX module providers autolinked
    return autolinking_cxxModuleProvider(name, jsInvoker);
}

extern "C" JNIEXPORT void JNICALL
Java_com_facebook_react_ReactPackage_initializeCxxModules(JNIEnv *env, jclass)
{
    facebook::react::DefaultTurboModuleManagerDelegate::registerComponentsFromEntryPoint();
}
