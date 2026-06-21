#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_io_github_vserghei_pcl_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "PCL Android ARM64 native bridge is loaded";
    return env->NewStringUTF(hello.c_str());
}
