/*
Detects SIFT features in two images and finds matches between them.

Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

@version 1.1.2-20100521
*/
#include "match.h"


/********************************** Main *************************************/


feature* doSIFT(char* img1_file, char* img2_file)
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

feature* checkMatch(struct feature *feat1, struct feature *feat2, int n1, int n2)
{
    struct kd_node* kd_root;
    struct feature ** nbrs;
    int i, k, m = 0;
    double d0, d1 = 0;
    struct feature *feat;
    feature* finalFea;
    
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
    
//    finalFea = (feature*)malloc(sizeof(feature));
    finalFea = new feature;
    finalFea = feat1;
    export_features(feat1_file, finalFea, n1-m);
	kdtree_release( kd_root );
	return finalFea;
}

void selectKeyFrames (char* videoFileDirectory, char* xmlFileDirectory)
{
    DIR *dirVideo, *dirXML;
    struct dirent *dirV;
    
    stringstream ss2;
    string s2;
    
    
    if((dirVideo = opendir(videoFileDirectory)) == NULL)
    {
        cout << "ERROR: Wrong DIrectory"<< endl;
        return;
    }
    if((dirXML = opendir(videoFileDirectory)) == NULL)
    {
        cout << "ERROR: Wrong DIrectory"<< endl;
        return;
    }
    
    /*
     * recursively read the video directory
     */
    while ((dirV = readdir(dirVideo)) != NULL) 
    {
        stringstream ss;
        string s;
        stringstream ss2;
        string s2;
        ss2 << xmlFileDirectory;
        ss2 >> s2;
        ss << dirV->d_name;
        ss >> s;
        
        stringstream ss1;
        string s1;
        ss1 << videoFileDirectory << "/" << dirV->d_name;
        ss1 >> s1;
        
        cout << "PASS: 1"<< endl;
        
        size_t pos = s.find(".mp4");
        /*
         *  find the video file
         */
        if(pos != string::npos)
        {
            cout << "PASS: 2"<< endl;
            /*
             * get video file's name
             * and used to find the respective .txt file
             */
            s2.append(s);
            s2.append(".txt");
            cout << s2<< endl;
            ifstream targetxmlfile(s2.c_str());
            /*
             *  find the xml file related to video file
             */
//            cout << s2.c_str() << endl;
            if (targetxmlfile.is_open()) 
            {
                int num = 0;
                cout << "PASS: 3"<< endl;
                /*
                 * read the first line of .txt file to initial the array to store keyframe info
                 */
                string line = "";
                getline(targetxmlfile, line);
                int max = atoi(line.c_str());
                int *keyframe = new int[max * 3];
                
                /*
                 * put keyframe info to array
                 */
                while (!targetxmlfile.eof()) 
                {
                    line = "";
                    num++;
                    getline(targetxmlfile, line);
                    int i = atoi(line.c_str());
                    keyframe[num-1] = i;
                }
                cout << "PASS: 4"<< endl;
                /*
                 * extract keyframes according to int array
                 * save them to a folder in image format
                 * one folder one video file
                 * each image is named in such format
                 * [user-defined directory][videofilename]_[shotnumber]_[1 or 2 or 3].[imageformat]
                 */
                CvCapture* cap = NULL;
                IplImage *cur_frame = NULL;
                IplImage *pre_frame = NULL;
                if(!(cap = cvCaptureFromFile(s1.c_str())))
                {
                    cout << "ERROR: reading video file";
                    exit(0);
                }
                
                int frame = 0;
                int key = 0;
                while (cap) 
                {
                    cout << "PASS: 5"<< endl;
                    cout << "key" << key << endl;;
                    
                    /*
                     * keyframes location
                     */
                    stringstream ss3;
                    string s3;
                    ss3 << keyframeDir;
                    ss3 >> s3;
                    s3.append(s);
                    s3.append("_");
                    
                    cur_frame = cvQueryFrame(cap);
                    int ii = key;
                    if(frame == keyframe[key] && key < max * 3)
                    {
                        stringstream temp;
                        string st;
                        temp << (key/3+1);
                        temp << "_";
                        temp << key % 3 + 1 << ".jpg";
                        temp >> st;
                        s3.append(st);
                        cout << "PASS: 6"<< endl;
                        
                        cvSaveImage(s3.c_str(), cur_frame);
                        key++;
                    }
                    
                    else if(keyframe[key] == keyframe[ii-1])
                    {
                        stringstream temp;
                        string st;
                        temp << (key/3+1);
                        temp << "_";
                        temp << key % 3 + 1 << ".jpg";
                        temp >> st;
                        s3.append(st);
                        cout << "PASS: 7"<< endl;
                        
                        cvSaveImage(s3.c_str(), pre_frame);
                        key++;
                        frame--;
                    }
                    frame ++;
                    pre_frame = cur_frame;
                    
                    cout << "frame: "<< frame << endl;
                    if( key == max * 3)
                    {
                        break;
                    }
                    s3.clear();
                    ss3.clear();
                }
            }
        }
    }
}

int main()
{
    
//    doSIFT(img1_file, img2_file);
    selectKeyFrames(videoFileDirectory, xmlFile);
    return 0;
}
