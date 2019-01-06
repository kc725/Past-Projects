#include <jni.h>
#include <string>
#include "CCalculator.h"

extern "C"
JNIEXPORT jfloat JNICALL Java_edu_cornell_calculator_MainActivity_add(JNIEnv *env, jobject, jfloat x, jfloat y){
    CCalculator calculator;
    return calculator.add(x, y);
}
extern "C"
JNIEXPORT jfloat JNICALL Java_edu_cornell_calculator_MainActivity_subtract(JNIEnv *env, jobject, jfloat x, jfloat y){
    CCalculator calculator;
    return calculator.subtract(x, y);
}
extern "C"
JNIEXPORT jfloat JNICALL Java_edu_cornell_calculator_MainActivity_multiply(JNIEnv *env, jobject, jfloat x, jfloat y){
    CCalculator calculator;
    return calculator.multiply(x, y);
}
extern "C"
JNIEXPORT jfloat JNICALL Java_edu_cornell_calculator_MainActivity_divide(JNIEnv *env, jobject, jfloat x, jfloat y){
    CCalculator calculator;
    return calculator.divide(x, y);
}

    /*   JNIEXPORT jstring JNICALL
Java_edu_cornell_calculator_MainActivity_stringFromJNI(JNIEnv *env,jobject) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
*/
