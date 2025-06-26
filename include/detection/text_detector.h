#ifndef TEXT_DETECTOR_H
#define TEXT_DETECTOR_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace detection {

    struct DetectedText {
        std::string text;
        cv::Rect bbox;
    };

    std::vector<DetectedText> detect_text_and_boxes(const cv::Mat& image);

    void draw_text_boxes(cv::Mat& frame, const std::vector<DetectedText>& detected_texts);

    cv::Mat preprocess_image(const cv::Mat& input_image);

} // namespace detection

#endif // TEXT_DETECTOR_H