//
// Created by Qiwei Huang on 2022/8/31.
//

#pragma once

#include "material/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "utility/rtweekend.h"

class ImageTexture : public Texture {
 public:
  const static int bytes_per_pixel = 3;

  ImageTexture()
      : data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

  explicit ImageTexture(const char* filename) {
    auto components_per_pixel = bytes_per_pixel;

    data_ = stbi_load(filename, &width_, &height_, &components_per_pixel,
                      components_per_pixel);

    if (!data_) {
      std::cerr << "ERROR: Could not load texture image file '" << filename
                << "'." << std::endl;
      width_ = height_ = 0;
    }
    bytes_per_scanline_ = bytes_per_pixel * width_;
  }

  ~ImageTexture() { delete data_; }

  [[nodiscard]] Color Value(double u, double v, const Vec3& p) const override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data_ == nullptr) {
      return {0, 1, 1};
    }

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = Clamp(u, 0.0, 1.0);
    v = 1.0 - Clamp(v, 0.0, 1.0);

    auto i = static_cast<int>(u * width_);
    auto j = static_cast<int>(v * height_);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width_) i = width_ - 1;
    if (j >= height_) j = height_ - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data_ + j * bytes_per_scanline_ + i * bytes_per_pixel;

    return {color_scale * pixel[0], color_scale * pixel[1],
            color_scale * pixel[2]};
  }

 private:
  unsigned char* data_;
  int width_{}, height_{};
  int bytes_per_scanline_;
};

#pragma endregion  // RAY_TRACING_ONE_WEEK_IMAGE_TEXTURE_H
