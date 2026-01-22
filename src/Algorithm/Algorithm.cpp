#include "Algorithm.h"

#include <opencv2/highgui.hpp>

cv::Mat Algorithm::pixmapToGrayMat(const QPixmap &pixmap) {
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);

    cv::Mat rgba(
        img.height(),
        img.width(),
        CV_8UC4,
        const_cast<uchar*>(img.bits()),
        img.bytesPerLine()
    );

    cv::Mat gray;
    cv::cvtColor(rgba, gray, cv::COLOR_RGBA2GRAY);

    return gray;
}

QPixmap Algorithm::matToPixmap(const cv::Mat& mat) {
    QImage img;

    if (mat.type() == CV_8UC1) {
        // Grayscale
        img = QImage(
            mat.data,
            mat.cols,
            mat.rows,
            mat.step,
            QImage::Format_Grayscale8
        );
    } else if (mat.type() == CV_8UC3) {
        // BGR → RGB
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

        img = QImage(
            rgb.data,
            rgb.cols,
            rgb.rows,
            rgb.step,
            QImage::Format_RGB888
        );

        return QPixmap::fromImage(img.copy());
    } else if (mat.type() == CV_8UC4) {
        // BGRA → RGBA
        cv::Mat rgba;
        cv::cvtColor(mat, rgba, cv::COLOR_BGRA2RGBA);

        img = QImage(
            rgba.data,
            rgba.cols,
            rgba.rows,
            rgba.step,
            QImage::Format_RGBA8888
        );

        return QPixmap::fromImage(img.copy());
    } else {
        return QPixmap(); // Unsupported format
    }

    // Grayscale path
    return QPixmap::fromImage(img.copy());
}

std::vector<std::vector<cv::Point>> Algorithm::getContours(const cv::Mat &image) {
    cv::Mat img = image.clone();

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

cv::Mat Algorithm::canny(const cv::Mat &image, int brightness, int contrast, int min_threshold, int max_threshold, int blur, int canny_low, int canny_high, int denoise, int expand) {
    cv::Mat img = image.clone();

    // --- Brightness & Contrast ---
    cv::Mat adjustedImage;
    cv::convertScaleAbs(img, adjustedImage, (contrast + 100.0f) / 100.0f, brightness + 200);

    // --- Thresholding ---
    cv::Mat th;
    cv::threshold(adjustedImage, th, max_threshold, max_threshold, cv::THRESH_TRUNC);
    cv::threshold(th, th, min_threshold, 0, cv::THRESH_TOZERO);

    // --- Denoise ---
    int k = denoise * 2 + 1;
    if (k > 1) cv::GaussianBlur(th, th, cv::Size(k, k), 0);

    // --- Edges ---
    cv::Mat edges;
    cv::Canny(th, edges, canny_low, canny_high);

    // --- Expand Edges ---
    if (expand > 1) {
        cv::Mat kernel = cv::Mat::ones(expand, expand, CV_8U);
        cv::morphologyEx(edges, edges, cv::MORPH_CLOSE, kernel);
    }

    return edges;
}