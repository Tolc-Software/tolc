// NOTE: This is the JNI generated header for this library
#include "com_tolc_mymodule_MyModule.h"

#include <iostream>
#include <map>
#include <string>

// Functions defined in the library trying to bind
namespace {
std::string returnStringFromCPP() {
  return "Hello from C++!";
}

void passStringToCPP(std::string& s) {
  s += " World!";
}

}    // namespace

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_tolc_mymodule_MyModule
 * Method:    passJavaObjectToCPP
 * Signature: (Lcom/tolc/mymodule/MyModule/DataFromJava;)V
 */
JNIEXPORT void JNICALL Java_com_tolc_mymodule_MyModule_passJavaObjectToCPP(
    JNIEnv* env,
    jobject thisObject,
    jobject dataFromJavaObject) {
  jclass dataFromJavaObjectClass = env->GetObjectClass(dataFromJavaObject);

  jfieldID a = env->GetFieldID(dataFromJavaObjectClass, "a", "I");
  jfieldID b = env->GetFieldID(dataFromJavaObjectClass, "b", "I");
  env->SetIntField(dataFromJavaObject, a, (jint)100);
  env->SetIntField(dataFromJavaObject, b, (jint)200);
}

/*
 * Class:     com_tolc_mymodule_MyModule
 * Method:    passStringToCPP
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_tolc_mymodule_MyModule_passStringToCPP(JNIEnv* env,
                                                jobject thisObject,
                                                jstring s) {
  char const* cstring_s = env->GetStringUTFChars(s, NULL);
  std::string cppstring_s = std::string(cstring_s);
  passStringToCPP(cppstring_s);
  return env->NewStringUTF(cppstring_s.c_str());
}

/*
 * Class:     com_tolc_mymodule_MyModule
 * Method:    returnStringFromCPP
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_tolc_mymodule_MyModule_returnStringFromCPP(JNIEnv* env,
                                                    jobject thisObject) {
  return env->NewStringUTF(returnStringFromCPP().c_str());
}

namespace MyModule {
struct Car {
  std::string model;
  double maxSpeed;
};
}    // namespace MyModule

/*
 * Class:     com_tolc_mymodule_MyModule
 * Method:    tolcNewMyModuleCar
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL
Java_com_tolc_mymodule_MyModule_tolcNewMyModuleCar(JNIEnv* env,
                                                   jclass rootClass) {
  // jfieldID tolcNativeMap = env->GetFieldID(rootClass, "tolcNativeMap", "J");
  // jlong j = env->GetLongField(rootClass, tolcNativeMap);

  MyModule::Car* nativeCar = new MyModule::Car();

  std::cout << "Created a car at: " << nativeCar << '\n';

  return reinterpret_cast<jlong>(nativeCar);
}

/*
 * Class:     com_tolc_mymodule_MyModule
 * Method:    tolcDeleteMyModuleCar
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_com_tolc_mymodule_MyModule_tolcDeleteMyModuleCar(JNIEnv*,
                                                      jclass,
                                                      jlong carPointer) {
  MyModule::Car* cppNativeCar = reinterpret_cast<MyModule::Car*>(carPointer);

  std::cout << "Deleting a car at: " << cppNativeCar << '\n';

  delete cppNativeCar;
}

#ifdef __cplusplus
}
#endif
