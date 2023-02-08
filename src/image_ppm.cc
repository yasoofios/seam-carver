#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

// ImagePPM::ImagePPM() {

// }

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
  //  real_height_ = height_;
  //  real_width_ = width_;
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }


Pixel ImagePPM::GetPixel(int row, int col) const {
  return pixels_[row][col];
}

  // returns the width of the image

  // returns the max color value of the image
  int ImagePPM::GetMaxColorValue() const {
    return max_color_value_;
  }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }
  //  real_height_ = height_;
  //  real_width_ = width_;
  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) { //real_height_
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
  // real_height_ = 0;
  //  real_width_ = 0;
}


std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3" << std::endl;
  os << image.GetWidth() << " " << image.GetHeight() << std::endl;
  os << image.GetMaxColorValue() << std::endl;
  for (int row = 0; row < image.GetHeight(); row++) {
    for (int col  = 0; col < image.GetWidth(); col++) {
      os << image.GetPixel(row, col).GetRed() << std::endl;
       os << image.GetPixel(row, col).GetGreen() << std::endl;
        os << image.GetPixel(row, col).GetBlue() << std::endl;
    }
  }

  return os;
}
std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  unsigned long space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  //  image.real_height_ = image.height_;
  //  image.real_width_ = image.width_;
  return is;
}

void ImagePPM::DeleteHSeam(const int* bruh) {
  Pixel** arr = new Pixel*[height_-1];
  for (int i = 0; i < height_ - 1; i++) {
    arr[i] = new Pixel[width_];
  }

  for (int i = 0; i < width_; i++) {
    int track = 0;
    for (int j = 0; j < height_ - 1; j++) {
      if (bruh[i] == j) {
        track+=1;
      }
      arr[j][i] = GetPixel(j + track, i);
    }
  }
 
    for (int j = 0; j < height_ ;j++) {
      delete[] pixels_[j];
    }
  delete[] pixels_;
  
  //delete[] pixels_[height_-1];
  pixels_ = arr;
  height_--;
}

void ImagePPM::DeleteVSeam(const int* bruh) {
  for (int i = 0; i < height_;i++) {
    for (int j = bruh[i]; j < width_- 1; j++) {
      pixels_[i][j] = pixels_[i][j+ 1];
    }
  }
  width_--;
}