//
//  facialDetection.hpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//
//  Viola-Jones paper
//  AND
//  https://medium.com/datadriveninvestor/understanding-and-implementing-the-viola-jones-image-classification-algorithm-85621f7fe20b
//

#ifndef facialDetection_hpp
#define facialDetection_hpp

#include <stdio.h>
#include <stdlib.h>

#include "debug.hpp"
#include "texture.hpp"

namespace AIFRED
{
    namespace FacialDetection
    {
        typedef float Percent;
		
			// wrote myself :D no help
		template<typename T>
		void arrayHeapAllocate(T ***var, const int x, const int y);
		
        
        struct Feature
        {
            
            
            Percent faceHaarAverage;
            Percent faceCorrect;
            
            Percent nonFaceHaarAverage;
            Percent nonFaceCorrect;
            
            float faceHaarTotal = 0;
            float nonFaceHaarTotal = 0;
			
			float thresholdmin = 0;
			float threshold = 0;
			int parity = 1;
            
            int x = 0;
            int y = 0;
            
            int w = 0;
            int h = 0;
            
            int type = 0;
            
        };
        
        struct FeatureImage
        {
            Feature *featuresSorted;
        };
		
		struct Eval
		{
			Percent succPerc; // percent of classifiers passed
			Percent errorPerc; // error of image
		};
		
		struct EvalImage : Eval
		{
			int x, y;
		};
		
	
		
		struct FDScanner
		{
			float sum(int x, int y, int width, int height);
			float abs(float in);
			
			void makeIntegralImage();
			
			int sizeX, sizeY; // should be const
			
			
			Render::Texture::colourByte** greyMap;
			
			u_int64_t** integralImage;
		};
		class FDSingleScanner : public FDScanner
        {
			void initSetFeatures(int imxs, int imys);
			void init();
			
			void process();
			
			int outLength;
			
            Feature* sort(Feature *features, int length);
			void prune(Feature *features, int length, Feature *outFeatures, int &outLength, Percent threshold);
			void draw(Feature target);
			Eval eval();
			
			
			const bool integralImageProvided;
            
        public:
			int mxs, mys; // modifiable for image to be cropped
			
            Feature* imageFeatures;
            FeatureImage imageFeaturesEval;
			
			void process(Render::Texture::colourByte** igreyMap);
			void process(u_int64_t ** iintegralImage); // called every training sample
			
            void evaluateImage(int iteration, bool b_sort); // called at the end of training
			Eval evaluate(Render::Texture::colourByte** igreyMap); // called during every comparison
			Eval evaluate(u_int64_t** iintegralImage);
			
			Render::Texture::ColourRGB **toImage();
			
            FDSingleScanner(int inSizeX, int inSizeY);
			//crop
			FDSingleScanner(int inSizeX, int inSizeY, int cropMxs, int cropMys);
            ~FDSingleScanner();
            
            friend struct Classifiers;
        };
		
			// Facial Detection Scanner
		class FDFullScanner : public FDScanner
		{
		public:
			
			FDSingleScanner *fss;
			FDFullScanner(FDSingleScanner &ifss, int inSizeX, int inSizeY);
			~FDFullScanner();
			
			EvalImage *findFaces(Render::Texture::Image &image, Percent strideLength, int threshold);
			EvalImage *evalAll(Render::Texture::colourByte **image, Percent strideLength);
		};
        
        struct Classifiers
        {
            static bool A (int x, int y, int width, int height, int threshold, FDSingleScanner& image);
            static bool B (int x, int y, int width, int height, int threshold, FDSingleScanner& image);
            static bool C (int x, int y, int width, int height, int threshold, FDSingleScanner& image);
            static bool D (int x, int y, int width, int height, int threshold, FDSingleScanner& image);
            
            static float A (int x, int y, int width, int height, FDSingleScanner& image);
            static float B (int x, int y, int width, int height, FDSingleScanner& image);
            static float C (int x, int y, int width, int height, FDSingleScanner& image);
            static float D (int x, int y, int width, int height, FDSingleScanner& image);
        };
    }
}

#endif /* facialDetection_hpp */
