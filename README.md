# PCL Android ARM64

Prebuilt Point Cloud Library (PCL) binaries packaged as an Android AAR for `arm64-v8a`.

This project is meant for Android apps that already use CMake/NDK and want to use PCL without building PCL, Boost, Eigen, FLANN, and LZ4 from source.

## What is included

- Android AAR library module: `pclibrary`
- Prefab metadata so native consumers can use `find_package(pclibrary REQUIRED CONFIG)`
- Prebuilt native dependencies for `arm64-v8a`
- Sample Android app that links against the published native target

Supported ABI:

```text
arm64-v8a
```

Minimum Android API:

```text
minSdk 29
```

## Gradle dependency

Add GitHub Packages to the consuming project's `settings.gradle`:

```groovy
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven {
            url = uri("https://maven.pkg.github.com/V-Serghei/pcl-binaries-android-armv8")
            credentials {
                username = providers.gradleProperty("gpr.user").orNull ?: System.getenv("GITHUB_ACTOR")
                password = providers.gradleProperty("gpr.key").orNull ?: System.getenv("GITHUB_TOKEN")
            }
        }
    }
}
```

Add the dependency to the Android app module:

```groovy
dependencies {
    implementation "io.github.vserghei:pcl-android-arm64:1.0.1"
}
```

Enable Prefab and restrict the app to the supported ABI:

```groovy
android {
    buildFeatures {
        prefab true
    }

    defaultConfig {
        ndk {
            abiFilters "arm64-v8a"
        }
    }
}
```

## CMake usage

In the consuming app's `CMakeLists.txt`:

```cmake
add_library(native-lib SHARED native-lib.cpp)

find_package(pclibrary REQUIRED CONFIG)

target_link_libraries(native-lib
        pclibrary::pclibrary
        log)
```

After this, native code can include PCL headers:

```cpp
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
```

## Local development

Build the library and sample:

```powershell
.\gradlew.bat clean :pclibrary:assembleRelease :sample:assembleDebug
```

Publish to GitHub Packages from a release workflow:

```powershell
.\gradlew.bat :pclibrary:publish
```

For local publishing tests:

```powershell
.\gradlew.bat :pclibrary:publishToMavenLocal
```

## Sample app

The `sample` module demonstrates the intended integration:

- `sample/build.gradle` enables Prefab and depends on `:pclibrary`
- `sample/src/main/cpp/CMakeLists.txt` imports `pclibrary`
- `sample/src/main/cpp/native-lib.cpp` creates a PCL point cloud and runs a voxel-grid filter

Run:

```powershell
.\gradlew.bat :sample:assembleDebug
```

## Publishing coordinates

```text
groupId:    io.github.vserghei
artifactId: pcl-android-arm64
version:    1.0.1
```

## Limitations

- Only `arm64-v8a` is included.
- Apps targeting other ABIs must either exclude them or provide matching PCL builds.
- This package is intended for native C++/CMake consumers. The Kotlin `NativeLib` class is only a small load-test bridge, not a high-level PCL API.
- GitHub Packages may require authentication even for public packages.

## Security note

Do not put GitHub tokens in Gradle files. Use GitHub Actions `GITHUB_TOKEN`, repository secrets, environment variables, or local `gradle.properties`.
