#include "ofMain.h"

#define NUM 30

class ofApp : public ofBaseApp {
    
    
    ofBoxPrimitive box, outBox;
    ofEasyCam cam;
    ofShader lightshader, shadowshader;
    
    ofVec3f pos[NUM];
    float time;
    
    ofSpherePrimitive sphere, lightsphere;
    ofPlanePrimitive plane;
    ofMatrix4x4 model;
    ofTexture texture;
    
    ofVec3f lightPos;
    
    GLuint depthMapFBO;
    GLuint depthCubemap;
    
    ofCamera pointDepthCam;
    ofMatrix4x4 lightViewProjectionMatrix[6], lightViewMatrix[6], lightProjectionMatrix;

    
    const float near = 1.0f;
    const float far = 350.0f;
    const int SHADOW_WIDTH = 1024;
    const int SHADOW_HEIGHT = 1024;

    const int WIDTH = ofGetWidth();
    const int HEIGHT = ofGetHeight();
    //--------------------------------------------------------------
    void setup()
    {
        lightshader.load("shaders/point_shadows_depth.vert",
                         "shaders/point_shadows_depth.frag",
                         "shaders/point_shadows_depth.geom");
        
//        lightshader.load("shaders/point_shadows_depth.vert",
//                         "shaders/point_shadows_depth.frag");
        
        shadowshader.load("shaders/point_shadows");
        
        
        cam.setNearClip(0);
        cam.setFarClip(1500);
        
        lightsphere.set(5, 10);
        box.setResolution(1);
        
        box.set(30);
        outBox.set(200);
        plane.set(800, 800);
        
        glEnable(GL_DEPTH_TEST);
        
        ofDisableArbTex();
        ofLoadImage(texture, "tex/concrete.jpg");
        
        
        glGenFramebuffers(1, &depthMapFBO);
        // Create depth cubemap texture
        
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (GLuint i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // Attach cubemap as depth map FBO's color buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "Framebuffer not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        
        float R = 200;
        for(int i = 0; i < NUM; i++)
        {
            pos[i] = ofVec3f(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
        }
        
        lightPos = ofVec3f(0.0, 70.0, 0.0);
        
        lightProjectionMatrix.makePerspectiveMatrix(90, 1.0, near, far);

        
        cout << box.getMesh().getMode() << endl;
    }
    
    //--------------------------------------------------------------
    void update()
    {
        time = ofGetElapsedTimef();
        lightPos.y = 40 * sin(time);
        
        lightViewMatrix[0].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 1.0,  0.0,  0.0), ofVec3f(0.0, -1.0,  0.0));
        lightViewMatrix[1].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f(-1.0,  0.0,  0.0), ofVec3f(0.0, -1.0,  0.0));
        lightViewMatrix[2].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  1.0,  0.0), ofVec3f(0.0,  0.0,  1.0));
        lightViewMatrix[3].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0, -1.0,  0.0), ofVec3f(0.0,  0.0, -1.0));
        lightViewMatrix[4].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  0.0,  1.0), ofVec3f(0.0, -1.0,  0.0));
        lightViewMatrix[5].makeLookAtViewMatrix(lightPos, lightPos + ofVec3f( 0.0,  0.0, -1.0), ofVec3f(0.0, -1.0,  0.0));
        
        for(int i = 0; i < 6; i++)
            lightViewProjectionMatrix[i] = lightViewMatrix[i] * lightProjectionMatrix;
    }
    
    //--------------------------------------------------------------
    void draw()
    {
        glViewport(0.0, 0.0, SHADOW_WIDTH, SHADOW_HEIGHT);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        cam.begin();
        ofMatrix4x4 viewMatrix = ofGetCurrentViewMatrix();
        ofDrawSphere(lightPos.x, lightPos.y, lightPos.z, 2);
        cam.end();
        
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
            lightshader.begin();
            lightshader.setUniform3f("lightPos", lightPos);
            lightshader.setUniform1f("far", far);
            lightshader.setUniformMatrix4f("projection", lightProjectionMatrix);
            
            for(int i = 0; i < 6; i++)
                lightshader.setUniformMatrix4f("light["+ to_string(i) +"].viewProjectionMatirx", lightViewProjectionMatrix[i]);
            
            {
                ofMatrix4x4 model;
                model.translate(0.0, 0.0, 0.0);
                //model.rotate(90, 1, 0, 0);
                lightshader.setUniformMatrix4f("model", model);
                glDisable(GL_CULL_FACE);
                outBox.draw();
                glEnable(GL_CULL_FACE);
            }
            
            for(int i = 0; i < NUM; i++)
            {
                ofMatrix4x4 model;
                model.rotate(time * 10.0 + i, 1, 0.4, 0.1);
                model.translate(pos[i]);
                lightshader.setUniformMatrix4f("model", model);
                box.draw();
            }
            lightshader.end();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        
        
        glViewport(0, 0, WIDTH, HEIGHT);
        shadowshader.begin();
        shadowshader.setUniformTexture("diffuseTexture", texture, 0);
        shadowshader.setUniformTexture("depthCubemap", GL_TEXTURE_CUBE_MAP, depthCubemap, 1);
        shadowshader.setUniformMatrix4f("view", viewMatrix);
        shadowshader.setUniformMatrix4f("projection", cam.getProjectionMatrix());
        shadowshader.setUniform3f("lightPos", lightPos);
        shadowshader.setUniform1f("far", far);
        
        {
            ofMatrix4x4 model;
            model.translate(0.0, 0.0, 0.0);
            //model.rotate(90, 1, 0, 0);
            shadowshader.setUniformMatrix4f("model", model);
            
            glDisable(GL_CULL_FACE); // Note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
            shadowshader.setUniform1i("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
            outBox.draw();
            shadowshader.setUniform1i("reverse_normals", 0); // And of course disable it
            glEnable(GL_CULL_FACE);
        }
        
        for(int i = 0; i < NUM; i++)
        {
            ofMatrix4x4 model;
            model.rotate(time * 10.0 + i, 1, 0.4, 0.1);
            model.translate(pos[i]);
            shadowshader.setUniformMatrix4f("model", model);
            box.draw();
        }
        
        shadowshader.end();
        
    }
    
    //--------------------------------------------------------------
    void keyPressed(int key)
    {
        
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key)
    {
        
    }
    
};


//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.width = 980;
    settings.height = 520;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
