//
//  main.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 22/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//
#include <stdio.h>
#include <string>
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



int main() {
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
    
    if (shader.init() == 1)
        return 1;
    

    bool init = false;
	bool end = false;
	int fa = 0;
	float avgEval = 0;
	float highest = 0, lowest = 0;
	int ihighest = 0, ilowest = 0;
	int a = 0;
    
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(shader.shader_programme, "myTextureSampler");

	char *home = getenv("HOME");
	const char *filename = "/Desktop/_final/face";
	
	
	char filenameNew[1024];
	{
		using namespace std;
		string ff = string(filename);
		printf("%d %d\n", 1, fa);
		strcpy(filenameNew, (home + ff + to_string(1) + string(".png")).c_str());
	}
	
	Render::Texture::Image image = Render::Texture::createImage(filenameNew);
	
	// create new GImage (to be processed)
	AIFRED::FacialDetection::FDScanner MainImage = AIFRED::FacialDetection::FDScanner(image);
	
	
	
	AIFRED::FacialDetection::GreyImage inImage(128, 128);
	inImage.initSetFeatures(100, 128); // crop image
    
    while(!glfwWindowShouldClose(shader.window)) {
        if (GLFW_PRESS == glfwGetKey(shader.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(shader.window, 1);
        }
		
        
        /*unsigned char pick_col[3][100][100];
        glReadPixels(841 , 270 , 100 , 100 , GL_RGB , GL_UNSIGNED_BYTE , pick_col);
        if (pick_col[0][0][0] + pick_col[1][0][0] + pick_col[2][0][0] == 0)
        {
            printf("a\n");
        }*/
		

		image.loadPNG("/Users/chaidhatchaimongkol/Downloads/t4.png");
		printf("%d\n", a);
		printf("%s\n", __TIME__);
		a++;
		
		{
			using namespace std;
			using namespace AIFRED::FacialDetection;
			
			if (a > totalImages)
			{
				if (!end
					&& a < 300)
				{
					string f = string("/Desktop/_final/nonface");
					printf("%d %d\n", a, fa);
					strcpy(filenameNew, (home + f + to_string(a - totalImages - 1) + string(".png")).c_str());
					printf("%s", filenameNew);
					
					image.loadPNG(filenameNew);
					//Texture::loadGreyImage(filenameNew, inImage);
					inImage.process(true);
					Eval ev = inImage.evaluate();
					float e = ev.failPerc;
					printf("avg %f\n", avgEval / (a - fa));
					printf("h %f %d\n", highest, ihighest);
					printf("l %f %d %d\n", lowest, ilowest, fa);
					
					//if (e > -50000 && e < 20000)
					if (e < 450)
					{
						printf("\n\n that's a face!\n");
					}
					else
					{
						printf("\n\n that's NOT a face!\n");
					}
					a++;
				}
				//end = true;
				if (autoTerminate)
					glfwSetWindowShouldClose(shader.window, 1);
				
			}
			else
			{
				string f = string(filename);
				printf("%d %d\n", a, fa);
				strcpy(filenameNew, (home + f + to_string(a) + string(".png")).c_str());
				
				//Texture::loadGreyImage(filenameNew, inImage);
				image.loadPNG(filenameNew);
				inImage.process(true);

				float e = 0;
				if (a != 60 && a != 3)
				{
					inImage.evaluateImage(a, true);
					Eval ev = inImage.evaluate();
					float ef = ev.failPerc;
					float ee = ev.evalPerc;
					if (ef >= 500 || ee > 10)
					{
						e = 0;
						printf("wow");
						fa++;
					}
					else
					{
						e = ee;
					}
				}
				if (e > 20000 || e < -50000)
					printf("wow\n");
				avgEval += e;
				if (e > highest)
				{
					highest = e;
					ihighest = a;
				}
				if (e < lowest)
				{
					lowest = e;
					ilowest = a;
				}
				a++;
				
			}
		}
		//Texture = Texture::loadTexture(inImage.greyMap);
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

