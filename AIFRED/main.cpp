//
//  main.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 22/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//
#include <stdio.h>
#include <string>
#include <cstdio>
#include <ctime>
#include <assert.h>

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include "AIFRED/facialDetection.hpp"

#include "render/texture.hpp"
#include "render/shader.hpp"

#include "debug.hpp"

#define PNG_DIMENSION 128;

Render::Shader shader;

Debug debug;
const bool autoTerminate = false; // false for debug/visual environment
const int totalImages = 200; // images to be processed

void glfw_error_callback(int error, const char* description) {
    debug.gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void initRendering(GLuint &TextureID)
{
	assert(debug.restart_gl_log());
	// start GL context and O/S window using the GLFW helper library
	debug.gl_log("starting GLFW\n%s\n", glfwGetVersionString());
	// register the error call-back function that we wrote, above
	glfwSetErrorCallback(glfw_error_callback);
	
	// uncomment these lines if on Apple OS xMouse
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// init shaders
	shader.init();
	
	// Get a handle for our "myTextureSampler" uniform
	TextureID  = glGetUniformLocation(shader.shader_programme, "myTextureSampler");
}

typedef std::string fileDirectory;

fileDirectory initFileDir(std::string filename)
{
	char *home = getenv("HOME");
	
	static char filenameNew[1024];
	{
		using namespace std;
		strcpy(filenameNew, (home + filename).c_str());
	}
	
	return filenameNew;
}
const char* loadFilename(fileDirectory iFileDir, int index)
{
	return (iFileDir + std::to_string(index) + std::string(".png")).c_str();
}

class Timer
{
	std::clock_t timer;
public:
	Timer()
	{
		timer = std::clock();
	}
	void reset()
	{
		timer = std::clock();
	}
	float getTime ()
	{
		return ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
	}
};



int main(int argc, const char *argv[]) {
	Timer timer;
	
	GLuint TextureID;
	initRendering(TextureID);
	
    

    bool init = false;
	bool end = false;
	int fa = 0;
	float avgEval = 0;
	float avgEval2 = 0;
	float highest = 0, lowest = 100000;
	int ihighest = 0, ilowest = 0;
	
	float highest2 = 0;
	int ihighest2 = 0;
	float lowest2 = 1000000;
	int ilowest2 = 0;
	int fa2 = 0;
	int a = 0;
	
	fileDirectory fileDirFace = initFileDir("/Desktop/_final/face");
	fileDirectory fileDirNonFace = initFileDir("/Desktop/_final/nonface");
	
	Render::Texture::Image image = Render::Texture::createImage(loadFilename(fileDirFace, 1));
	
	// create new GImage (to be processed)
	AIFRED::FacialDetection::FDScanner MainImage = AIFRED::FacialDetection::FDScanner(image);
	
	
	
	AIFRED::FacialDetection::FDSingleScanner inImage(128, 128, 100, 128);
    
    while(!glfwWindowShouldClose(shader.window)) {
        if (GLFW_PRESS == glfwGetKey(shader.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(shader.window, 1);
        }
		
        
        
		

		printf("face %d\n", a);
		printf("%f at %f fps\n", timer.getTime(), (a-200) / timer.getTime());
		
		Render::Texture::Image *imgtoPrint = &image;
		a++;
		
		float es = 0;
		float ef = 0;
		
		{
			using namespace std;
			using namespace AIFRED::FacialDetection;
			
			if (a > totalImages)
			{
				if (!end
					&& a < 300)
				{
					if (a == totalImages + 1)
					{
						inImage.evaluateImage(a, true);
					}
					
					
					
					image.loadPNG(loadFilename(fileDirFace, a - totalImages - 1));
					Eval ev = inImage.evaluate(image.toFDSingleScanner());
					float efail = ev.errorPerc;
					float esucc = ev.succPerc;
					
					float e = esucc; //
					
					
					
					if (esucc > 0.3)
					{
						printf("F that's a face! %f %f\n", efail, esucc);
						fa++;
					}
					else
					{
						printf("F that's NOT a face! %f %f\n", efail, esucc);
					}

					avgEval += e;
					if (e > highest){highest = e; ihighest = a;}
					if (e < lowest){lowest = e; ilowest = a;}
					
					image.loadPNG(loadFilename(fileDirNonFace, a - totalImages - 1));
					ev = inImage.evaluate(image.toFDSingleScanner());
					efail = ev.errorPerc;
					esucc = ev.succPerc;
					
					float e2 = esucc; //

					
					
					if (esucc > 0.3)
					{
						printf("N that's a face! %f %f\n", efail, esucc);
						fa2++;
					}
					else
					{
						printf("N that's NOT a face! %f %f\n", efail, esucc);
					}
					
					avgEval2 += e2;
					if (e2 > highest2){highest2 = e2; ihighest2 = a;}
					if (e2 < lowest2){lowest2 = e2; ilowest2 = a;}
					
					/*if (e < 450)
					{
						printf("\n\n that's a face!\n");
					}
					else
					{
						printf("\n\n that's NOT a face!\n");
					}*/
				}
				else
				{
					if (a == 301)
					{
						timer.reset();
						printf("\n\n---SUMMARY 2---\n\n");
						printf("True Pos  avg %f\n", avgEval / ((a-totalImages) - 1));
						printf("h %f %d\n", highest, ihighest);
						printf("l %f %d %d\n", lowest, ilowest, fa);
						
						printf("False Pos avg %f\n", avgEval2 / ((a-totalImages) - 1));
						printf("h %f %d\n", highest2, ihighest2);
						printf("l %f %d %d\n", lowest2, ilowest2, fa2);
					}
				
					system("screencapture -x ~/Desktop/scn.png");
					system("sips -c 256 256 ~/Desktop/scn.png");
					system("sips -z 128 128 ~/Desktop/scn.png");
					Render::Texture::Image out = Render::Texture::createImage(initFileDir("/Desktop/scn.png").c_str());
					out.loadPNG(initFileDir("/Desktop/scn.png").c_str());
					
					//image.loadPNG("/Users/chaidhatchaimongkol/Downloads/t4.png");
					
					
					Eval ev = inImage.evaluate(out.toFDSingleScanner());
					
					imgtoPrint = &out;
					 es = ev.succPerc;
					
					
				}
				//end = true;
				if (autoTerminate)
					glfwSetWindowShouldClose(shader.window, 1);
				
			}
			else
			{
				
				image.loadPNG(loadFilename(fileDirFace, a));
				inImage.process(image.toFDSingleScanner());

				
			}
		}
		//Texture = Texture::loadTexture(inImage.greyMap);
		image.data = inImage.toImage();
		

		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				image.data[x][y].R = (es * (255/0.4));
				if (es > 0.3)
				image.data[x][y].R = 255;
				image.data[x][y].G = (es * (255/0.4));
				image.data[x][y].B = (es * (255/0.4));}
			
		}
		//GLuint Texture = Render::Texture::loadImage(image);
		
		//image.cropImage(0, 0, 128, 128);
		GLuint Texture = Render::Texture::loadImage(image);
        
        
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
        glVertexAttribPointer(
                              0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, shader.vuv);
        glVertexAttribPointer(
                              1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                              2,                                // size : U+V => 2
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        shader.update();
        
        //glDeleteTextures(1, &Render::Texture::textureID);
        
        
        if (!init) { init = true; debug.gl_log("\nInitialised Successfully. %s %s \n", __DATE__, __TIME__);}
    }
	inImage.evaluateImage(a, true);
	debug.gl_log("\nFinished. Here's the results\n");
	debug.gl_log("\n");
	
	using namespace AIFRED::FacialDetection;
	Feature bestEval = inImage.imageFeaturesEval.featuresSorted[0];
	debug.gl_log("Best Eval Avg:\t%f\n", bestEval.faceHaarAverage);
	
	debug.gl_log("Best type:\t%d\n", bestEval.type);
	debug.gl_log("Best x:\t%d\n", bestEval.x);
	debug.gl_log("Best y:\t%d\n", bestEval.y);
	debug.gl_log("Best w:\t%d\n", bestEval.w);
	debug.gl_log("Best h:\t%d\n", bestEval.h);
	
	debug.gl_log("Face Avg:\t%f %%\n", inImage.imageFeatures[0].faceHaarAverage * 100);
	debug.gl_log("Face Correct:\t%f %%\n", inImage.imageFeatures[0].faceCorrect * 100);
	debug.gl_log("\nTerminate.\n");
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

