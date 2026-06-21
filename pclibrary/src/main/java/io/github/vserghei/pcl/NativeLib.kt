package io.github.vserghei.pcl

class NativeLib {
    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("pclibrary")
        }
    }
}
