<div align="center">

# PCL Android ARM64

### Prebuilt Point Cloud Library binaries for Android, packaged as an AAR with Prefab metadata.

[![Latest Release](https://img.shields.io/github/v/release/V-Serghei/pcl-binaries-android-armv8?label=GitHub%20Release&color=2ea44f)](https://github.com/V-Serghei/pcl-binaries-android-armv8/releases/latest)
[![License](https://img.shields.io/github/license/V-Serghei/pcl-binaries-android-armv8)](LICENSE)
[![Android](https://img.shields.io/badge/platform-Android-3DDC84?logo=android&logoColor=white)](https://developer.android.com/)
[![ABI](https://img.shields.io/badge/ABI-arm64--v8a-blue)](#requirements)
[![NDK](https://img.shields.io/badge/NDK-26.1.10909125-orange)](#required-ndk-version)
[![Maven Central](https://img.shields.io/badge/Maven%20Central-io.github.v--serghei%3Apcl--android--arm64-blue)](#maven-central)

</div>

## Overview

This repository provides prebuilt [Point Cloud Library](https://pointclouds.org/) binaries for Android apps that already use the NDK and CMake.

It lets Android projects use PCL without rebuilding PCL, Boost, Eigen, FLANN, and LZ4 from source.

The package is intended for native C++ Android consumers. It is not a high-level Kotlin or Java wrapper for PCL.

## Contents

- [Quick Start](#quick-start)
- [Requirements](#requirements)
- [Package Coordinates](#package-coordinates)
- [Bundled Native Libraries](#bundled-native-libraries)
- [Full Usage Example](#full-usage-example)
- [Required NDK Version](#required-ndk-version)
- [Sample App](#sample-app)
- [Publishing](#publishing)
- [Troubleshooting](#troubleshooting)

## Quick Start

### 1. Add Maven Central

Use Maven Central as the default integration path. It is public and does not require GitHub credentials.

```groovy
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}
```

### 2. Add The Dependency

```groovy
dependencies {
    implementation "io.github.v-serghei:pcl-android-arm64:1.0.3"
}
```

### 3. Enable Prefab And Pin The ABI

```groovy
android {
    ndkVersion "26.1.10909125"

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

### 4. Link From CMake

```cmake
add_library(native-lib SHARED native-lib.cpp)

find_package(pclibrary REQUIRED CONFIG)

target_link_libraries(native-lib
        pclibrary::pclibrary
        log)
```

### 5. Include PCL Headers

```cpp
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
```

## Requirements

<div align="center">

| Requirement | Value |
| --- | --- |
| Android ABI | `arm64-v8a` |
| Minimum SDK | `29` |
| Compile SDK used by this repo | `34` |
| Required NDK | `26.1.10909125` |
| CMake | `3.22.1` |
| Android Gradle Plugin | `8.5.1` |
| Gradle Wrapper | `8.10` |
| Package format | Android AAR with Prefab |

</div>

## Package Coordinates

<div align="center">

| Field | Value |
| --- | --- |
| Public repository | `mavenCentral()` |
| Group ID | `io.github.v-serghei` |
| Artifact ID | `pcl-android-arm64` |
| Current version | `1.0.3` |
| Maven coordinate | `io.github.v-serghei:pcl-android-arm64:1.0.3` |
| GitHub Packages fallback | `https://maven.pkg.github.com/V-Serghei/pcl-binaries-android-armv8` |

</div>

## Bundled Native Libraries

<div align="center">

| Component | Version | Notes |
| --- | --- | --- |
| PCL | `1.9.1` | Point Cloud Library static archives repackaged into `libpclibrary.so` |
| Boost | `1.70` | Required by PCL |
| Eigen | `3.3.7` | Header-only linear algebra dependency |
| FLANN | `1.9.1` | Required by PCL search and kdtree functionality |
| LZ4 | `1.9.1` | Compression dependency |
| OpenMP runtime | NDK-provided | Linked because the bundled PCL archives were built with OpenMP |

</div>

## Full Usage Example

Minimal Android app module `build.gradle`:

```groovy
plugins {
    id "com.android.application"
}

android {
    namespace "com.example.pclapp"
    compileSdk 34
    ndkVersion "26.1.10909125"

    defaultConfig {
        applicationId "com.example.pclapp"
        minSdk 29
        targetSdk 34

        externalNativeBuild {
            cmake {
                cppFlags "-std=c++17"
                arguments "-DANDROID_STL=c++_shared"
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
    implementation "io.github.v-serghei:pcl-android-arm64:1.0.3"
}
```

Minimal `CMakeLists.txt`:

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

## Required NDK Version

Install Android NDK `26.1.10909125` before building a consuming app.

In Android Studio:

```text
Tools -> SDK Manager -> SDK Tools -> NDK (Side by side)
```

Enable **Show Package Details**, select:

```text
26.1.10909125
```

Then click **Apply**.

Pin the same version in the Android module that uses this package:

```groovy
android {
    ndkVersion "26.1.10909125"
}
```

For example, in a consuming project with a `nativelib` module:

```powershell
.\gradlew.bat clean :nativelib:assembleDebug
```

This package was built and tested with NDK `26.1.10909125`. NDK 27 uses libc++ 18, where `std::binary_function` is fully removed. The bundled PCL and FLANN headers still reference `std::binary_function`, and NDK 26 still provides it in C++17 mode.

If you see this error, the consuming app is likely building with NDK 27:

```text
flann/util/heap.h:108:35: error: no template named 'binary_function' in namespace 'std'
```

Use NDK `26.1.10909125` and rebuild.

## Sample App

The `sample` module demonstrates the intended integration.

<div align="center">

| File | Purpose |
| --- | --- |
| `sample/build.gradle` | Enables Prefab, restricts ABI to `arm64-v8a`, and depends on `:pclibrary` |
| `sample/src/main/cpp/CMakeLists.txt` | Imports the native package with `find_package(pclibrary REQUIRED CONFIG)` |
| `sample/src/main/cpp/native-lib.cpp` | Creates a small point cloud and runs `pcl::VoxelGrid` |

</div>

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

Build both the library and sample:

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

## Publishing

### GitHub Packages

The repository publishes to GitHub Packages from `.github/workflows/gradle-publish.yml`.

Release flow:

1. Update `VERSION_NAME` in `gradle.properties`.
2. Commit and push the changes.
3. Merge into `main`.
4. Create a GitHub Release with a matching tag, for example `v1.0.3`.
5. The `Gradle Package` workflow builds the library and sample.
6. On release events, the workflow runs `:pclibrary:publish`.

Expected published coordinate:

```text
io.github.v-serghei:pcl-android-arm64:1.0.3
```

### Maven Central

Maven Central is the recommended public distribution channel because consumers can use `mavenCentral()` without GitHub tokens.

Central Portal does not accept a plain `.aar` file. The **Publish Component** dialog expects a signed Maven deployment bundle `.zip` that contains the AAR, POM, Gradle module metadata, sources jar, javadoc jar, checksums, and GPG signatures.

Create the upload bundle:

```powershell
.\gradlew.bat clean :pclibrary:mavenCentralBundle
```

The upload file will be created at:

```text
pclibrary/build/distributions/pcl-android-arm64-1.0.3-maven-central-bundle.zip
```

In `central.sonatype.com/publishing`:

1. Click **Publish Component**.
2. Use deployment name `pcl-android-arm64-1.0.3`.
3. Use description `PCL Android ARM64 AAR with Prefab metadata.`
4. Select `pclibrary/build/distributions/pcl-android-arm64-1.0.3-maven-central-bundle.zip`.
5. Click **Publish Component**.
6. Wait for validation.
7. If validation passes, click **Publish** on the deployment card.

### GPG Signing For Maven Central

Add local signing settings to `~/.gradle/gradle.properties`:

```properties
signingInMemoryKeyFile=C:/Users/YOUR_USER/.gradle/pcl-central-signing-key.asc
signingInMemoryKeyPassword=YOUR_GPG_KEY_PASSWORD
```

Do not commit this file or the exported private key.

## GitHub Packages Fallback

GitHub Packages can require authentication even for public packages. Use it only if the version you need is not available on Maven Central yet.

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

For local development, put credentials in the consuming project's `~/.gradle/gradle.properties`:

```properties
gpr.user=YOUR_GITHUB_USERNAME
gpr.key=YOUR_GITHUB_TOKEN_WITH_READ_PACKAGES
```

Do not commit tokens to this repository or to consuming apps.

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

### `std::binary_function` is missing in FLANN headers

Pin the consuming Android module to NDK `26.1.10909125`:

```groovy
android {
    ndkVersion "26.1.10909125"
}
```

Then install that NDK through Android Studio:

```text
Tools -> SDK Manager -> SDK Tools -> NDK (Side by side) -> Show Package Details -> 26.1.10909125 -> Apply
```

Rebuild:

```powershell
.\gradlew.bat clean :nativelib:assembleDebug
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

Never put GitHub tokens, Maven Central credentials, or GPG private keys in Gradle files. Use GitHub Actions `GITHUB_TOKEN`, repository secrets, environment variables, or local untracked `gradle.properties`.
