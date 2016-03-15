#include "fast.h"
#include <algorithm>

void FastFeatureDetector::detect(const Mat& img, std::vector<KeyPoint>& keypoints, const Mat& mask) {
  FAST(img, keypoints, threshold, non_max_suppression);
  KeyPointsMask(keypoints, mask);
}

bool MaskPredicate::operator() (const KeyPoint& key_pt) const {
  return mask.data[(int)(key_pt.y + 0.5f)][(int)(key_pt.x + 0.5f)] == 0;
}

static void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask) {
  if (mask.empty())
    return;

  keypoints.erase(std::remove_if(keypoints.begin(), keypoints.end(), MaskPredicate(mask)), keypoints.end());
}

void FAST(const Mat& img, std::vector<KeyPoint>& keypoints, int threshold, bool non_max_suppression) {
  threshold = std::min(std::max(threshold, 0), 255);
  keypoints.clear();
  int circle[25];
  get_circle(img.cols, circle);
  unsigned char threshold_tab[511];
  get_threshold_tab(threshold, threshold_tab);
  std::vector<std::vector<int>> score_buf(3, std::vector<int>(img.cols, 0));
  std::vector<std::vector<int>> position_buf(3, std::vector<int>(img.cols));
  std::vector<size_t> ncorners(3, 0);

  for (size_t i = 3; i < img.rows - 2; ++i) {
    int curr = i % 3, prev = (i - 1) % 3, pprev = (i - 2) % 3;
    score_buf[curr].clear();
    ncorners[curr] = 0;

    // When i == img.rows - 2, We only need to check the keypoints in previous row.
    if (i < img.rows - 3) {
      for (size_t j = 3; j < img.cols - 3; ++j) {
        const unsigned char* v = img.data[i] + j;
        unsigned char* tab = threshold_tab + 255 - v[0];

        unsigned char d = tab[v[circle[0]]] | tab[v[circle[8]]];

        if (!d)
          continue;

        d &= tab[v[circle[2]]] | tab[v[circle[10]]];
        d &= tab[v[circle[4]]] | tab[v[circle[12]]];
        d &= tab[v[circle[6]]] | tab[v[circle[14]]];
        
        if (!d)
          continue;

        d &= tab[v[circle[1]]] | tab[v[circle[9]]];
        d &= tab[v[circle[3]]] | tab[v[circle[11]]];
        d &= tab[v[circle[5]]] | tab[v[circle[13]]];
        d &= tab[v[circle[7]]] | tab[v[circle[15]]];

        if (d) {
          unsigned char threshold_flag{0};
          for (size_t k = 0, count = 0; k < 25; ++k) {
            if (v[circle[k]] - v[0] > threshold) {
              threshold_flag == 2 ? ++count : count = 1;
              threshold_flag = 2;
            } else if (v[circle[1]] - v[0] < -threshold) {
              threshold_flag == 1 ? ++count : count = 1;
              threshold_flag = 1;
            } else {
              count = 0;
              threshold_flag = 0;
            }

            // There's a keypoint, store the infos.
            if (count > 8) {
              score_buf[curr][j] = get_score_buf(v, circle);
              position_buf[curr][ncorners[curr]] = j;
              ++ncorners[curr];
              break;
            }
          }
        }
      }
    }

    // The first calculated row.
    if (i == 3)
      continue;

    // Select keypoints in previous row.
    for (size_t k = 0; k < ncorners[prev]; ++k) {
      int j = position_buf[prev][k];
      int score = score_buf[prev][j];
      if (!non_max_suppression ||
          (score > score_buf[prev][j - 1] && score > score_buf[prev][j + 1] &&
           score > score_buf[pprev][j - 1] && score > score_buf[pprev][j + 1] &&
           score > score_buf[curr][j - 1] && score > score_buf[curr][j + 1] &&
           score > score_buf[pprev][j] && score > score_buf[curr][j]))
        keypoints.push_back({(float)j, (float)(i - 1), 7.f, -1.f, (float)score});
    }
  }
}

static void get_circle(int img_cols, int* circle) {
  // The first point is three lines before the central point.
  // Other points are arranged clockwise.
  // Continue the circle in order not to miss continuous arcs
  // which include circle[0].
  circle[0] = circle[16] = -img_cols * 3;
  circle[1] = circle[17] = -img_cols * 3 + 1;
  circle[2] = circle[18] = -img_cols * 2 + 2;
  circle[3] = circle[19] = -img_cols + 3;
  circle[4] = circle[20] = 3;
  circle[5] = circle[21] = img_cols + 3;
  circle[6] = circle[22] = img_cols * 2 + 2;
  circle[7] = circle[23] = img_cols * 3 + 1;
  circle[8] = circle[24] = img_cols * 3;
  circle[9] = img_cols * 3 - 1;
  circle[10] = img_cols * 2 - 2;
  circle[11] = img_cols - 3;
  circle[12] = -3;
  circle[13] = -img_cols - 3;
  circle[14] = -img_cols * 2 - 2;
  circle[15] = -img_cols * 3 - 1;
}

static void get_threshold_tab(int threshold, unsigned char* threshold_tab) {
  for (size_t i = 0; i < 511; ++i) {
    int vd = i - 255;
    threshold_tab[i] = vd < -threshold ? 1 : vd > threshold ? 2 : 0;
  }
}

static int get_score_buf(const unsigned char* v, const int* circle) {
  int curr_score_buf{0};
  for (size_t i = 0; i < 16 ; ++i) {
    curr_score_buf += abs(v[circle[i]] - v[0]);
  }
  return curr_score_buf;
}