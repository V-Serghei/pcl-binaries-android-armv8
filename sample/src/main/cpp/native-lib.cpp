#include <jni.h>
#include <sstream>
#include <string>

#include <pcl/filters/voxel_grid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

extern "C" JNIEXPORT jstring JNICALL
Java_io_github_vserghei_pcl_sample_MainActivity_runPclSmokeTest(
        JNIEnv *env,
        jobject /* this */) {
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
    message << "PCL loaded through Prefab\n"
            << "Input points: " << cloud->size() << "\n"
            << "Filtered points: " << filtered.size();

    return env->NewStringUTF(message.str().c_str());
}
