#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

// returns the instance's image_
const ImagePPM& SeamCarver::GetImage() const {
  const ImagePPM& bruh = image_;
  return bruh;
}

// returns the image's height
int SeamCarver::GetHeight() const { return height_; }

// returns the image's width
int SeamCarver::GetWidth() const { return width_; }

// returns the energy of the pixel at row col in image_
int SeamCarver::GetEnergy(int row, int col) const {
  Pixel up{}, down{};
  Pixel left{};
  Pixel right{};
  if (row == 0) {
    up = image_.GetPixel((height_ - 1), col);
  } else {
    up = image_.GetPixel((row - 1), col);
  }
  if (row == height_ - 1) {
    down = image_.GetPixel((height_ - height_), col);
  } else {
    down = image_.GetPixel((row + 1), col);
  }
  if (col == 0) {
    left = image_.GetPixel(row, (width_ - 1));
  } else {
    left = image_.GetPixel(row, (col - 1));
  }
  if (col == width_ - 1) {
    right = image_.GetPixel(row, (width_ - width_));
  } else {
    right = image_.GetPixel(row, (col + 1));
  }
  int up_red = up.GetRed(), down_red = down.GetRed();
  int left_red = left.GetRed(), right_red = right.GetRed();
  int up_green = up.GetGreen(), down_green = down.GetGreen();
  int left_green = left.GetGreen(), right_green = right.GetGreen();
  int up_blue = up.GetBlue(), down_blue = down.GetBlue();
  int left_blue = left.GetBlue(), right_blue = right.GetBlue();
  int row_eng_red = (up_red - down_red) * (up_red - down_red);
  int row_eng_green = (up_green - down_green) * (up_green - down_green);
  int row_eng_blue = (up_blue - down_blue) * (up_blue - down_blue);
  int col_eng_red = (left_red - right_red) * (left_red - right_red);
  int col_eng_green = (left_green - right_green) * (left_green - right_green);
  int col_eng_blue = (left_blue - right_blue) * (left_blue - right_blue);
  int total_row = row_eng_red + row_eng_green + row_eng_blue;
  int total_col = col_eng_red + col_eng_green + col_eng_blue;
  int total = total_row + total_col;
  return total;
}
int* SeamCarver::GetHorizontalSeam() const {
  int** energy = new int*[height_];
  for (int i = 0; i < height_; i++) {
    energy[i] = new int[width_];
  }// eng array
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      energy[i][j] = GetEnergy(i, j);
    }
  }// vert seam
  int** seam_array = new int*[height_];//int** double_gay = new int*[height_];
  for (int i = 0; i < height_; i++) {
    seam_array[i] = new int[width_];
  }
  int width_index = width_ - 1;
  for (int cols = 0; cols < height_; cols++) {
    seam_array[cols][width_index] = energy[cols][width_index];
  }
  for (int cols = width_ - 2; cols > -1; cols--) { //int ro = height_- 1; ro > -1; ro--
  for (int ro = height_ -1; ro > -1; ro--) {
    int best = 0;//int down_right = seam_array[ro +1][cols +1];
    int down = seam_array[ro][cols + 1];//int down_left = seam_array[ro + 1][cols - 1];
    best = down;
    if (ro != 0 && down > seam_array[ro -1][cols + 1]) {
      best = seam_array[ro - 1][cols + 1];
    }
    if ( ro != height_ - 1 && down > seam_array[ro + 1][cols + 1]) {
      best = seam_array[ro + 1][cols + 1];
    }
    seam_array[ro][cols] = best + energy[ro][cols];
  }
}
 return HorSeamHelp(energy, seam_array);
}


int* SeamCarver::HorSeamHelp(int** energy, int** seam_array) const {
   int* bruh = new int[width_];
 int min = INT32_MAX;
 for (int i = 0; i < height_; i++) {
  if (min > seam_array[i][0]) {
    min = seam_array[i][0];
    bruh[0] = i;
  }
 }
 for (int i = 1 ; i < width_; i++) {
  int min = seam_array[bruh[i - 1]][i];
  int min_index = bruh[i-1];
  if (bruh[i-1] != 0 && min > seam_array[bruh[i-1]-1][i]) {
    min = seam_array[bruh[i-1]-1][i];
    min_index = bruh[i-1]-1;
  }
  if (bruh[i-1] != height_ -1 && min > seam_array[bruh[i-1]+1][i]) {
    min = seam_array[bruh[i-1]+1][i];
    min_index = bruh[i-1]+1;
  }
  bruh[i] = min_index;
  }
int i = 0;
  while(i < height_) {
    delete[] energy[i];
    i++; 
  }
  delete[] energy;
  int j = 0;
  while (j < height_) {
    delete[] seam_array[j];
    j++;
  }
  delete[] seam_array;
  return bruh;

}
int* SeamCarver::VertSeamHelp(int** energy, int** seam_array) const {
  int* bruh = new int[height_];
 int min = INT32_MAX;
 for (int i = 0; i < width_; i++) {
  if (min > seam_array[0][i]) {
    min = seam_array[0][i];
    bruh[0] = i;
  }
 }
 for (int i = 1 ; i < height_; i++) {
  int min = seam_array[i][bruh[i - 1]];
  int min_index = bruh[i-1];
  if (bruh[i-1] != 0 && min > seam_array[i][bruh[i-1]-1]) {
    min = seam_array[i][bruh[i-1]-1];
    min_index = bruh[i-1]-1;
  }
  if (bruh[i-1] != height_ -1 && min > seam_array[i][bruh[i-1]+1]) {
    min = seam_array[i][bruh[i-1]+1];
    min_index = bruh[i-1]+1;
  }
  bruh[i] = min_index;
  }
int i = 0;
  while(i < height_) {
    delete[] energy[i];
    i++; 
  }
  delete[] energy;
  int j = 0;
  while (j < height_) {
    delete[] seam_array[j];
    j++;
  }
  delete[] seam_array;
  return bruh;
}
int* SeamCarver::GetVerticalSeam() const {
  int** energy = new int*[height_];
  for (int i = 0; i < height_; i++) {energy[i] = new int[width_];}// eng array
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {energy[i][j] = GetEnergy(i, j);}
  }// vert seam
  int** seam_array = new int*[height_];//int** double_gay = new int*[height_];
  for (int i = 0; i < height_; i++) {seam_array[i] = new int[width_];}
  int height_index = height_ - 1;
  for (int cols = 0; cols < width_; cols++) {seam_array[height_index][cols] = energy[height_index][cols];}
  for (int ro = height_- 2; ro > -1; ro--) {
  for (int cols = 0; cols < width_; cols++) {
    int best = 0;//int down_right = seam_array[ro +1][cols +1];
    int down = seam_array[ro +1][cols];//int down_left = seam_array[ro + 1][cols - 1];
    best = down;
    if (cols != 0 && down > seam_array[ro + 1][cols - 1]) { best = seam_array[ro + 1][cols - 1];}
    if ( cols != width_ - 1 && down > seam_array[ro + 1][cols + 1]) { best = seam_array[ro + 1][cols + 1];}
    seam_array[ro][cols] = best + energy[ro][cols];
  }
}
 return VertSeamHelp(energy, seam_array);
}
//  void SeamCarver::Delete(int** seam_array, int** energy ) {
//   int i = 0;
//   while(i < height_) {
//     delete[] energy[i];
//     i++; 
//   }
//   delete[] energy;

//   int j = 0;
//   while (j < height_) {
//     delete[] seam_array[j];
//     j++;
//   }
//   delete[] seam_array;

//  }
void SeamCarver::RemoveHorizontalSeam() { 

  int* bruh = GetHorizontalSeam();
  image_.DeleteHSeam(bruh);
  delete[] bruh;
  height_--; 
  }

// removes one vertical seam in image_. example:
//
//
void SeamCarver::RemoveVerticalSeam() { 
int* bruh = GetVerticalSeam();
  image_.DeleteVSeam(bruh);
  delete[] bruh;
  width_--; 
  }
/**
 * Add any helper methods you may need
 */
