#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <QPixmap>

namespace Algorithm {
    cv::Mat pixmapToGrayMat(const QPixmap& pixmap);
    QPixmap matToPixmap(const cv::Mat &image);

    std::vector<std::vector<cv::Point>> getContours(const cv::Mat &image, bool closeGaps = false, int gapClosingKernelSize = 3);
    cv::Mat canny(const cv::Mat &image, int brightness, int contrast, int min_threshold, int max_threshold, int blur, int canny_low, int canny_high, int denoise, int expand, cv::Vec3b outlineColor = cv::Vec3b(0, 0, 255), bool showBackground = true, float outlineOpacity = 1.0f);
}