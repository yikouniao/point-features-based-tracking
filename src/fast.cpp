#include "fast.h"
#include <algorithm>

using namespace std;

void FastDetector::Detect(
    const Mat& img, std::vector<KeyPoint>& keypoints) {
  keypoints.clear();
  threshold = min(max(threshold, 0), 255);
  vector<int> circle(25);
  GetCircleOffsets(img.step, circle);
  vector<uchar> thres_tab(511);
  GetThresTab(threshold, thres_tab);
  vector<vector<int>> score_buf(3, vector<int>(img.cols, 0));
  vector<vector<int>> position_buf(3, vector<int>(img.cols));
  vector<size_t> ncorners(3, 0);

  for (size_t i = 3; i < img.rows - 2; ++i) {
    int curr = i % 3, prev = (i - 1) % 3, pprev = (i - 2) % 3;
    fill(score_buf[curr].begin(), score_buf[curr].end(), 0);
    ncorners[curr] = 0;

    // When i == img.rows - 2, We only need to check the keypoints
    // in previous row.
    if (i < img.rows - 3) {
      for (size_t j = 3; j < img.cols - 3; ++j) {
        const uchar* v = &img(i, j);
        vector<uchar>::const_iterator tab = thres_tab.begin() + 255 - *v;

        // Check whether there exists 9 contiguous pixels in the circle
        uchar d = *(tab + v[circle[0]]) | *(tab + v[circle[8]]);

        if (!d)
          continue;

        d &= *(tab + v[circle[2]]) | *(tab + v[circle[10]]);
        d &= *(tab + v[circle[4]]) | *(tab + v[circle[12]]);
        d &= *(tab + v[circle[6]]) | *(tab + v[circle[14]]);
        
        if (!d)
          continue;

        d &= *(tab + v[circle[1]]) | *(tab + v[circle[9]]);
        d &= *(tab + v[circle[3]]) | *(tab + v[circle[11]]);
        d &= *(tab + v[circle[5]]) | *(tab + v[circle[13]]);
        d &= *(tab + v[circle[7]]) | *(tab + v[circle[15]]);

        if (d) {
          uchar threshold_flag{0};
          for (size_t k = 0, count = 0; k < 25; ++k) {
            uchar v_diff = v[circle[k]] - *v;
            if (v_diff > threshold) {
              threshold_flag == 2 ? ++count : count = 1;
              threshold_flag = 2;
            } else if (v_diff < -threshold) {
              threshold_flag == 1 ? ++count : count = 1;
              threshold_flag = 1;
            } else {
              count = 0;
              threshold_flag = 0;
            }

            // There's a keypoint, store the infos.
            if (count > 8) {
              score_buf[curr][j] = GetFastScore(v, circle);
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

static void GetCircleOffsets(int img_step, std::vector<int>& circle) {
  int t = -img_step * 3; // temp
  circle[0] = circle[16] = t;
  circle[1] = circle[17] = t + 1;
  circle[15] = t - 1;

  t += img_step; // -img_step * 2
  circle[2] = circle[18] = t + 2;
  circle[14] = t - 2;

  t = -img_step; // -img_step
  circle[3] = circle[19] = t + 3;
  circle[13] = t - 3;

  circle[4] = circle[20] = 3;
  circle[12] = -3;

  t = img_step; // img_step
  circle[5] = circle[21] = t + 3;
  circle[11] = t - 3;

  t += img_step; // img_step * 2

  circle[6] = circle[22] = t + 2;
  circle[10] = t - 2;

  t += img_step; // img_step * 3
  circle[7] = circle[23] = t + 1;
  circle[8] = circle[24] = t;
  circle[9] = t - 1;
}

static void GetThresTab(
    int threshold, std::vector<uchar>& thres_tab) {
  for (size_t i = 0; i < 511; ++i) {
    int vd = i - 255;
    thres_tab[i] = vd < -threshold ? 1 : vd > threshold ? 2 : 0;
  }
}

static int GetFastScore(
    const uchar* v, const std::vector<int>& circle) {
  int curr_score_buf{0};
  for (size_t i = 0; i < 16 ; ++i) {
    curr_score_buf += abs(v[circle[i]] - *v);
  }
  return curr_score_buf;
}