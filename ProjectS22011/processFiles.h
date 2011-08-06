//
//  processFiles.h
//  ProjectS22011
//
//  Created by Nathan CHEN on 6/08/11.
//  Copyright 2011 The University of Sydney. All rights reserved.
//

#ifndef ProjectS22011_processFiles_h
#define ProjectS22011_processFiles_h

#include <fstream>
#include "match.h"
using namespace std;

/*
 *  extract key frames
 *  input: directory stores video files, file with indicate the keyframe info
 *  output: key frames and store them in different folders (one folder for one video file)
 */
void selectKeyFrames (char*, char*);

/*
 *  perform sift on all key frames
 *  input: a main directory, which contains multiple folders
 *  output: sift descriptor .txt file for one shot
 */
void generateSIFTForOneShot(char*);





#endif
