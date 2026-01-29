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
    if (image.empty()) {
        return std::vector<std::vector<cv::Point>>();
    }
    
    cv::Mat img = image.clone();
    
    // Convert to grayscale if needed
    if (img.channels() == 3) {
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    } else if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);
    }
    
    // Ensure single channel
    if (img.channels() != 1) {
        // qWarning() << "Failed to convert image to grayscale for contour detection";
        return std::vector<std::vector<cv::Point>>();
    }
    
    // Apply threshold to create binary image (if not already binary)
    cv::Mat binary;
    cv::threshold(img, binary, 127, 255, cv::THRESH_BINARY);
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    return contours;
}

cv::Mat Algorithm::canny(const cv::Mat &image, int brightness, int contrast, int min_threshold, int max_threshold, int blur, int canny_low, int canny_high, int denoise, int expand, cv::Vec3b outlineColor, bool showBackground, float outlineOpacity) {
    cv::Mat img = image.clone();

    // --- Brightness & Contrast ---
    cv::Mat adjustedImage;
    cv::convertScaleAbs(img, adjustedImage, (contrast + 100.0f) / 100.0f, brightness);

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

    // --- Background ---
    cv::Mat result;
    if (showBackground) {
        cv::cvtColor(th, result, cv::COLOR_GRAY2BGR);
    } else {
        result = cv::Mat::zeros(th.size(), CV_8UC3);
    }

    // --- Color + opacity ---
    cv::Mat coloredEdges(result.size(), CV_8UC3, outlineColor);
    cv::Mat edgeMask;
    edges.convertTo(edgeMask, CV_32F, 1.0 / 255.0);

    float alpha = std::clamp(outlineOpacity, 0.0f, 1.0f);

    cv::Mat resultF, edgesF;
    result.convertTo(resultF, CV_32F);
    coloredEdges.convertTo(edgesF, CV_32F);

    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            float m = edgeMask.at<float>(y, x) * alpha;
            if (m > 0.0f) {
                resultF.at<cv::Vec3f>(y, x) =
                    resultF.at<cv::Vec3f>(y, x) * (1.0f - m) +
                    edgesF.at<cv::Vec3f>(y, x) * m;
            }
        }
    }

    resultF.convertTo(result, CV_8UC3);
    return result;
}