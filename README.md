# point-features-based tracking
## Tracking based on ORB.

Most of the ORB code is based on the paper below. The rest refers to what the authors contributed to OpenCV3.10.

Ethan Rublee, Vincent Rabaud, Kurt Konolige, Gary Bradski, ORB: an efficient alternative to SIFT or SURF, Proceeding ICCV'11 Proceedings of the 2011 International Conference on Computer Vision, 2564-2571

## The tracking system

    read the object coordinates;
    set the first frame as the reference frame;
    calculate keypoints and descriptors for reference frame;
    while (input a new frame) {
      calculate keypoints and descriptors for the real-time frame;
      match their descriptors;
      if (matches.size() < threshold) { // threshold is 20 here
        match reference frame with the last 3 frames before the current one;
        set the frame with most valid matches as the new reference frame;
      }
      affine transformation; // Get the best transformation by sequential clustering
      update object coordinates;
    }
