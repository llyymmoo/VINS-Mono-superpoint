/**
 * @file    SuperPoint.hpp
 *
 * @author  btran
 *
 */

#pragma once

#include <opencv2/opencv.hpp>
#include "OrtSessionHandler.h"
#include "Utility_Ort.h"
#include <vector>

using KeyPointAndDesc = std::pair<std::vector<cv::KeyPoint>, cv::Mat>;

namespace Ort
{
class SuperPoint : public OrtSessionHandler
{
 public:
    static constexpr int64_t IMG_H = 480;
    static constexpr int64_t IMG_W = 640;
    static constexpr int64_t IMG_CHANNEL = 1;

    using OrtSessionHandler::OrtSessionHandler;

   //  SuperPoint(std::string _modelPath, int _gpuIdx, std::vector<std::vector<int64_t>> _inputShapes) :
   //             Ort::OrtSessionHandler(_modelPath, _gpuIdx, _inputShapes) {}

    void preprocess(float* dst,                     //
                    const unsigned char* src,       //
                    const int64_t targetImgWidth,   //
                    const int64_t targetImgHeight,  //
                    const int numChannels) const;

    std::vector<int> nmsFast(const std::vector<cv::KeyPoint>& keyPoints, int height, int width,
                             int distThresh = 2) const;

    /**
     *  @brief detect super point
     *
     *  @return vector of detected key points
     */
    std::vector<cv::KeyPoint> getKeyPoints(const std::vector<Ort::OrtSessionHandler::DataOutputType>& inferenceOutput,
                                           int borderRemove, float confidenceThresh) const;

    /**
     *  @brief estimate super point's keypoint descriptor
     *
     *  @return keypoint Mat of shape [num key point x 256]
     */
    cv::Mat getDescriptors(const cv::Mat& coarseDescriptors, const std::vector<cv::KeyPoint>& keyPoints, int height,
                           int width, bool alignCorners) const;


    KeyPointAndDesc processOneFrame(const cv::Mat& inputImg, int maxnums, float* dst, int borderRemove = 4,
                                    float confidenceThresh = 0.015, bool alignCorners = true, int distThresh = 2);
};
}  // namespace Ort
