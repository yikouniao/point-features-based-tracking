#include "fast.h"
#include <algorithm>

void FastFeatureDetector::detect(
    const Mat& img, std::vector<KeyPoint>& keypoints, const Mat& mask) {
  FAST(img, keypoints, threshold, non_max_suppression);
  KeyPointsMask(keypoints, mask);
}

void FAST(const Mat& img, std::vector<KeyPoint>& keypoints, int threshold,
          bool non_max_suppression) {
  threshold = std::min(std::max(threshold, 0), 255);
  keypoints.clear();
  std::vector<int> circle(25);
  get_circle(img.step, circle);
  std::vector<unsigned char> thres_tab(511);
  get_thres_tab(threshold, thres_tab);
  std::vector<std::vector<int>> score_buf(3, std::vector<int>(img.cols, 0));
  std::vector<std::vector<int>> position_buf(3, std::vector<int>(img.cols));
  std::vector<size_t> ncorners(3, 0);

  for (size_t i = 3; i < img.rows - 2; ++i) {
    int curr = i % 3, prev = (i - 1) % 3, pprev = (i - 2) % 3;
    std::fill(score_buf[curr].begin(), score_buf[curr].end(), 0);
    ncorners[curr] = 0;

    // When i == img.rows - 2, We only need to check the keypoints
    // in previous row.
    if (i < img.rows - 3) {
      for (size_t j = 3; j < img.cols - 3; ++j) {
        const unsigned char* v = &img(i, j);
        unsigned int tab = 255 - v[0];

        unsigned char d = thres_tab[tab + v[circle[0]]] |
                          thres_tab[tab + v[circle[8]]];

        if (!d)
          continue;

        d &= thres_tab[tab + v[circle[2]]] | thres_tab[tab + v[circle[10]]];
        d &= thres_tab[tab + v[circle[4]]] | thres_tab[tab + v[circle[12]]];
        d &= thres_tab[tab + v[circle[6]]] | thres_tab[tab + v[circle[14]]];
        
        if (!d)
          continue;

        d &= thres_tab[tab + v[circle[1]]] | thres_tab[tab + v[circle[9]]];
        d &= thres_tab[tab + v[circle[3]]] | thres_tab[tab + v[circle[11]]];
        d &= thres_tab[tab + v[circle[5]]] | thres_tab[tab + v[circle[13]]];
        d &= thres_tab[tab + v[circle[7]]] | thres_tab[tab + v[circle[15]]];

        if (d) {
          unsigned char threshold_flag{0};
          for (size_t k = 0, count = 0; k < 25; ++k) {
            if (v[circle[k]] - v[0] > threshold) {
              threshold_flag == 2 ? ++count : count = 1;
              threshold_flag = 2;
            } else if (v[circle[k]] - v[0] < -threshold) {
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
           score > score_buf[pprev][j - 1] && score > score_buf[pprev][j] &&
           score > score_buf[pprev][j + 1] && score > score_buf[curr][j]) &&
           score > score_buf[curr][j - 1] && score > score_buf[curr][j + 1])
        keypoints.push_back(
          {(float)j, (float)(i - 1), 7.f, -1.f, (float)score});
    }
  }
}

static void get_circle(int img_step, std::vector<int>& circle) {
  // The first point is three lines before the central point.
  // Other points are arranged clockwise.
  // Continue the circle in order not to miss continuous arcs
  // which include circle[0].
  circle[0] = circle[16] = -img_step * 3;
  circle[1] = circle[17] = -img_step * 3 + 1;
  circle[2] = circle[18] = -img_step * 2 + 2;
  circle[3] = circle[19] = -img_step + 3;
  circle[4] = circle[20] = 3;
  circle[5] = circle[21] = img_step + 3;
  circle[6] = circle[22] = img_step * 2 + 2;
  circle[7] = circle[23] = img_step * 3 + 1;
  circle[8] = circle[24] = img_step * 3;
  circle[9] = img_step * 3 - 1;
  circle[10] = img_step * 2 - 2;
  circle[11] = img_step - 3;
  circle[12] = -3;
  circle[13] = -img_step - 3;
  circle[14] = -img_step * 2 - 2;
  circle[15] = -img_step * 3 - 1;
}

static void get_thres_tab(int threshold,
                          std::vector<unsigned char>& thres_tab) {
  for (size_t i = 0; i < 511; ++i) {
    int vd = i - 255;
    thres_tab[i] = vd < -threshold ? 1 : vd > threshold ? 2 : 0;
  }
}

static int get_score_buf(const unsigned char* v,
                         const std::vector<int>& circle) {
  int curr_score_buf{0};
  for (size_t i = 0; i < 16 ; ++i) {
    curr_score_buf += abs(v[circle[i]] - v[0]);
  }
  return curr_score_buf;
}