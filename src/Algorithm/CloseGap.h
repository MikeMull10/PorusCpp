#pragma once

#include <opencv2/opencv.hpp>
#include <QPolygonF>

static cv::Mat polygonToMask(const QPolygonF& poly, int padding = 10);
static void closeMaskGap(cv::Mat& mat, int size = 7);
static std::vector<cv::Point> extractContour(const cv::Mat& mat);
static QPolygonF contourToPolygon(const std::vector<cv::Point>&, const QRectF&, int padding = 10);
QPolygonF closePolygon(const QPolygonF& input, int padding = 10, int size = 7);
