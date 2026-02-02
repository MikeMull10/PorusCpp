#include "CloseGap.h"

static cv::Mat polygonToMask(const QPolygonF& poly, int padding) {
    QRectF bounds = poly.boundingRect();

    int w = std::ceil(bounds.width())  + 2 * padding;
    int h = std::ceil(bounds.height()) + 2 * padding;

    cv::Mat mask = cv::Mat::zeros(h, w, CV_8UC1);

    std::vector<cv::Point> pts;
    pts.reserve(poly.size());

    for (const QPointF& p : poly) {
        pts.emplace_back(
            int(p.x() - bounds.left() + padding),
            int(p.y() - bounds.top()  + padding)
        );
    }

    std::vector<std::vector<cv::Point>> contours{pts};
    cv::fillPoly(mask, contours, cv::Scalar(255));

    return mask;
}

static void closeMaskGaps(cv::Mat& mask, int size) {
    if (size % 2 == 0) size++;

    // Kernel size controls max gap size
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_ELLIPSE,
        cv::Size(size, size)
    );

    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
}

static std::vector<cv::Point> extractLargestContour(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(
        mask,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    if (contours.empty())
        return {};

    // Keep largest area contour
    size_t best = 0;
    double bestArea = 0;

    for (size_t i = 0; i < contours.size(); ++i) {
        double a = cv::contourArea(contours[i]);
        if (a > bestArea) {
            bestArea = a;
            best = i;
        }
    }

    return contours[best];
}

static QPolygonF contourToPolygon(const std::vector<cv::Point>& contour, const QRectF& bounds, int padding) {
    QPolygonF poly;

    for (const cv::Point& p : contour) {
        poly.append(QPointF(
            p.x + bounds.left() - padding,
            p.y + bounds.top()  - padding
        ));
    }

    // Explicitly close
    if (!poly.isEmpty())
        poly.append(poly.first());

    return poly;
}

QPolygonF closePolygon(const QPolygonF& input, int padding, int size) {
    if (input.size() < 5)
        return input;

    QRectF bounds = input.boundingRect();

    cv::Mat mask = polygonToMask(input, padding);
    closeMaskGaps(mask, size);

    std::vector<cv::Point> contour = extractLargestContour(mask);
    if (contour.empty())
        return input;

    return contourToPolygon(contour, bounds, padding);
}
