# PCL Android ARM64

Prebuilt [Point Cloud Library](https://pointclouds.org/) binaries for Android, packaged as an AAR with Prefab metadata.

This repository is for Android apps that already use the NDK/CMake and want to use PCL without rebuilding PCL, Boost, Eigen, FLANN, and LZ4 from source.

## Quick Start

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

Add the package to the Android app module:

```groovy
dependencies {
    implementation "io.github.vserghei:pcl-android-arm64:1.0.2"
}
```

Enable Prefab and restrict the app to `arm64-v8a`:

```groovy
android {
    buildFeatures {
        prefab true
    }

    defaultConfig {
        minSdk 29

        ndk {
            abiFilters "arm64-v8a"
        }
    }
}
```

Link the native package from the app's `CMakeLists.txt`:

```cmake
add_library(native-lib SHARED native-lib.cpp)

find_package(pclibrary REQUIRED CONFIG)

target_link_libraries(native-lib
        pclibrary::pclibrary
        log)
```

Use PCL headers in C++:

```cpp
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
```

## Package Coordinates

| Field | Value |
| --- | --- |
| Maven repository | `https://maven.pkg.github.com/V-Serghei/pcl-binaries-android-armv8` |
| Group ID | `io.github.vserghei` |
| Artifact ID | `pcl-android-arm64` |
| Current version | `1.0.2` |
| Android ABI | `arm64-v8a` |
| Minimum SDK | `29` |
| Package format | Android AAR with Prefab |

## Bundled Native Libraries

| Component | Version | Notes |
| --- | --- | --- |
| PCL | `1.9.1` | Point Cloud Library static archives repackaged into `libpclibrary.so` |
| Boost | `1.70` | Required by PCL |
| Eigen | `3.3.7` | Header-only linear algebra dependency |
| FLANN | `1.9.1` | Required by PCL search/kdtree functionality |
| LZ4 | `1.9.1` | Compression dependency |
| OpenMP runtime | NDK-provided | Linked because the bundled PCL archives were built with OpenMP |

## Android Build Versions

| Tool | Version |
| --- | --- |
| Android Gradle Plugin | `8.5.1` |
| Gradle Wrapper | `8.10` |
| Kotlin Gradle Plugin | `1.9.0` |
| Compile SDK | `34` |
| CMake | `3.22.1` |
| Tested NDK | `26.1.10909125` |

## Full Usage Example

Minimal app module `build.gradle`:

```groovy
plugins {
    id "com.android.application"
}

android {
    namespace "com.example.pclapp"
    compileSdk 34

    defaultConfig {
        applicationId "com.example.pclapp"
        minSdk 29
        targetSdk 34

        externalNativeBuild {
            cmake {
                cppFlags "-std=c++17"
            }
        }

        ndk {
            abiFilters "arm64-v8a"
        }
    }

    buildFeatures {
        prefab true
    }

    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
            version "3.22.1"
        }
    }
}

dependencies {
    implementation "io.github.vserghei:pcl-android-arm64:1.0.2"
}
```

Minimal app `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.22.1)

project("pcl_app")

add_library(native-lib SHARED native-lib.cpp)

find_package(pclibrary REQUIRED CONFIG)

target_link_libraries(native-lib
        pclibrary::pclibrary
        log)
```

Minimal native smoke test:

```cpp
#include <jni.h>
#include <sstream>

#include <pcl/filters/voxel_grid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_pclapp_MainActivity_runPclSmokeTest(JNIEnv* env, jobject) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
    cloud->width = 8;
    cloud->height = 1;
    cloud->is_dense = true;
    cloud->points.resize(cloud->width * cloud->height);

    for (std::size_t i = 0; i < cloud->points.size(); ++i) {
        cloud->points[i].x = static_cast<float>(i) * 0.01f;
        cloud->points[i].y = static_cast<float>(i) * 0.02f;
        cloud->points[i].z = static_cast<float>(i) * 0.03f;
    }

    pcl::PointCloud<pcl::PointXYZ> filtered;
    pcl::VoxelGrid<pcl::PointXYZ> voxel_grid;
    voxel_grid.setInputCloud(cloud);
    voxel_grid.setLeafSize(0.03f, 0.03f, 0.03f);
    voxel_grid.filter(filtered);

    std::ostringstream message;
    message << "Input points: " << cloud->size()
            << ", filtered points: " << filtered.size();

    return env->NewStringUTF(message.str().c_str());
}
```

## Authentication For GitHub Packages

GitHub Packages may require authentication even for public packages.

For local development, put credentials in the consuming project's `~/.gradle/gradle.properties`:

```properties
gpr.user=YOUR_GITHUB_USERNAME
gpr.key=YOUR_GITHUB_TOKEN_WITH_READ_PACKAGES
```

Do not commit tokens to this repository or to consuming apps.

## Sample App

The `sample` module demonstrates the intended integration:

| File | Purpose |
| --- | --- |
| `sample/build.gradle` | Enables Prefab, restricts ABI to `arm64-v8a`, and depends on `:pclibrary` |
| `sample/src/main/cpp/CMakeLists.txt` | Imports the native package with `find_package(pclibrary REQUIRED CONFIG)` |
| `sample/src/main/cpp/native-lib.cpp` | Creates a small point cloud and runs `pcl::VoxelGrid` |

Build the sample:

```powershell
.\gradlew.bat :sample:assembleDebug
```

The generated APK is written to:

```text
sample/build/outputs/apk/debug/sample-debug.apk
```

## Local Development

Build the release AAR:

```powershell
.\gradlew.bat :pclibrary:assembleRelease
```

Build the sample app:

```powershell
.\gradlew.bat :sample:assembleDebug
```

Build both:

```powershell
.\gradlew.bat :pclibrary:assembleRelease :sample:assembleDebug
```

Test local Maven publishing:

```powershell
.\gradlew.bat :pclibrary:publishToMavenLocal
```

The release AAR is written to:

```text
pclibrary/build/outputs/aar/pclibrary-release.aar
```

## Release And Publishing

The repository publishes to GitHub Packages from `.github/workflows/gradle-publish.yml`.

Release flow:

1. Update `VERSION_NAME` in `gradle.properties`.
2. Commit and push the changes.
3. Merge into `main`.
4. Create a GitHub Release with a matching tag, for example `v1.0.2`.
5. The `Gradle Package` workflow builds the library and sample.
6. On release events, the workflow runs `:pclibrary:publish`.

Expected published coordinate:

```text
io.github.vserghei:pcl-android-arm64:1.0.2
```

## What This Package Is Not

This package is not a high-level Kotlin/Java PCL API.

It is a native C++ package for Android/CMake consumers. The Kotlin `NativeLib` class in `pclibrary` is only a small native load-test bridge.

## Limitations

- Only `arm64-v8a` is included.
- Apps targeting other ABIs must exclude them or provide matching PCL builds.
- The bundled PCL build is `1.9.1`; newer PCL APIs may not be available.
- The AAR is large because it packages PCL headers and a native shared library.
- GitHub Packages access may require a GitHub token with `read:packages`.

## Troubleshooting

### `find_package(pclibrary REQUIRED CONFIG)` fails

Check that the app module has Prefab enabled:

```groovy
android {
    buildFeatures {
        prefab true
    }
}
```

### App builds for unsupported ABIs

Restrict the ABI:

```groovy
android {
    defaultConfig {
        ndk {
            abiFilters "arm64-v8a"
        }
    }
}
```

### GitHub Packages returns 401 or 403

Configure credentials in `~/.gradle/gradle.properties`:

```properties
gpr.user=YOUR_GITHUB_USERNAME
gpr.key=YOUR_GITHUB_TOKEN_WITH_READ_PACKAGES
```

### `SDK location not found`

Create a local, untracked `local.properties` file:

```properties
sdk.dir=C\:/Users/YOUR_USER/AppData/Local/Android/Sdk
```

### Windows reports locked build files

Stop Gradle daemons and rebuild:

```powershell
.\gradlew.bat --stop
.\gradlew.bat :sample:assembleDebug --no-daemon
```

## Security

Never put GitHub tokens in Gradle files. Use GitHub Actions `GITHUB_TOKEN`, repository secrets, environment variables, or local `gradle.properties`.
