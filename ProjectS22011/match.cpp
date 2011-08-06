/*
Detects SIFT features in two images and finds matches between them.

Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

@version 1.1.2-20100521
*/
#include "match.h"


/********************************** Main *************************************/


int main(int argc, char ** argv)
{

	IplImage* img1, *img2;
	struct feature* feat1, * feat2;
    
    int n1, n2 = 0;

	img1 = cvLoadImage( img1_file, 1 );
	if( ! img1 )
		fatal_error("unable to load image from %s", img1_file );
    
	fprintf( stderr, "Finding features in %s...\n", img1_file );
	n1 = sift_features( img1, &feat1 );
    
    img2 = cvLoadImage( img2_file, 1 );
    if( ! img2 )
    fatal_error("unable to load image from %s", img2_file );
    
	fprintf( stderr, "Finding features in %s...\n", img2_file );
    
    n2 = sift_features(img2, &feat2);
    
//    export_features(feat1_file, feat1, n1);
//    export_features(feat2_file, feat2, n2);
    checkMatch(feat1, feat2, n1, n2);
	
	free( feat1 );
	free( feat2 );
	return 0;
}

void checkMatch(struct feature *feat1, struct feature *feat2, int n1, int n2)
{
    struct kd_node* kd_root;
    struct feature ** nbrs;
    int i, k, m = 0;
    double d0, d1 = 0;
    struct feature *feat;
    
    kd_root = kdtree_build(feat2, n2 );
    
    nbrs = (feature**) malloc(sizeof(feature*));
    
	for( i = 0; i < n1; i++ )
	{
		feat = feat1 + i;
		k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
            
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{
                feat = NULL;
				m++;
			}
		}
		free( nbrs );
	}

    fprintf( stderr, "Found %d total matches\n", m );
    
    export_features(feat1_file, feat1, n1-m);
	kdtree_release( kd_root );
	free( feat1 );
	free( feat2 );
}
