//
//  linkedlist.h
//  ProjectS22011
//
//  Created by Nathan CHEN on 7/08/11.
//  Copyright 2011 The University of Sydney. All rights reserved.
//

#include "imgfeatures.h"
#include <iostream.h>

using namespace std;

struct featureList
{
    char* filename;
    feature* feat;
    featureList* next;
};


featureList* createnew(char* filename, feature* feat);

featureList* addtotop(featureList* original, featureList* newfl);
