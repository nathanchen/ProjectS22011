//
//  linkedlist.cpp
//  ProjectS22011
//
//  Created by Nathan CHEN on 7/08/11.
//  Copyright 2011 The University of Sydney. All rights reserved.
//

#include "linkedlist.h"


featureList* createnew(char* filename, feature* feat)
{
    featureList *fl = (featureList*)malloc(sizeof(featureList));
    strcpy(fl->filename, filename);
    fl->feat = feat;
    fl->next = NULL;
    return fl;
}

featureList* addtotop(featureList* original, featureList* newfl)
{
    newfl->next = original;
    return newfl;
}