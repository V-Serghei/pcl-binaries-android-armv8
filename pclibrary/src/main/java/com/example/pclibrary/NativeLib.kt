package com.example.pclibrary

class NativeLib {

    /**
     * A native method that is implemented by the 'pclibrary' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'pclibrary' library on application startup.
        init {
            System.loadLibrary("pclibrary")
        }
    }
}