/** @file PointCloud.h
 *  @brief Contrete implimentation of pointcloud stabilization methods.
 * 
 *  @author Ao Y. Yu 
 *  @bug No known bugs.
 */

#ifndef _POINTCLOUDSTABILIZATION_H
#define _POINTCLOUDSTABILIZATION_H

#include <iostream>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

typedef pcl::PointCloud<pcl::PointXYZ> point_cloud_t;

class PointCloudStabilization{
    public:
        PointCloudStabilization();
        ~PointCloudStabilization();
    private:
};

#endif
