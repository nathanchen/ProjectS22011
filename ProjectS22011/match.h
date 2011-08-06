//
//  match.h
//  sift
//
//  Created by Nathan CHEN on 18/05/11.
//  Copyright 2011 The University of Sydney. All rights reserved.
//
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <stdio.h>
#include <iostream.h>
#include <string.h>

using namespace std;

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.36

/******************************** Globals ************************************/

char img1_file[] = "/Users/natechen/Desktop/box.pgm";
char img2_file[] = "/Users/natechen/Desktop/scene.pgm";

char feat1_file[] = "/Users/natechen/Desktop/1.txt";
char feat2_file[] = "/Users/natechen/Desktop/2.txt";

void checkMatch(struct feature *feat1, struct feature *feat2, int n1, int n2);
