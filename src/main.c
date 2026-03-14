#ifdef _WIN32
#include <windows.h>
#endif

#include <math.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GL/gl.h>

#include "cpup/io.h"
#include "cpup/vec.h"
#include "cpup/math.h"
#include "cpup/types.h"
#include "cpup/model.h"
#include "cpup/shader.h"
#include "cpup/window.h"
#include "cpup/inputmanager.h"

#include "cpup/scene.h"

#include "ball.h"
#include "paddle.h"
#include "trail.h"

AppContext app;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (InitCanis() > 0)
        return 1;

    app.windowWidth = 600;
    app.windowHeight = 600;
    
    if (InitWindow(&app) > 0)
        return 1;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Scene* scene = SceneInit();
    app.scene = scene;
    
    Image iconImage = IOLoadImage("assets/textures/canis_engine_icon.tga");
    Image containerImage = IOLoadImage("assets/textures/container.tga");
    Image circleImage = IOLoadImage("assets/textures/circle.tga");
    Image squareImage = IOLoadImage("assets/textures/square.tga");
    
    // build and compile our shader program
    u32 shaderProgram = GenerateShaderFromFiles("assets/shaders/logo.vs", "assets/shaders/logo.fs");
    u32 gridShader = GenerateShaderFromFiles("assets/shaders/grid.vs", "assets/shaders/grid.fs");
    u32 noiseShader = GenerateShaderFromFiles("assets/shaders/noise.vs", "assets/shaders/noise.fs");

    printf("shaderID: %i\n", shaderProgram);
    printf("shaderID: %i\n", gridShader);
    printf("shaderID: %i\n", noiseShader);

    float ve[] = {
        // positions            // texture coords
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f  // top left 
    };
    unsigned int in[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    float f[] = {
        1, 1, 0, 1, 1,
        1, -1, 0, 1, 0,
        -1, -1, 0, 0, 0,
        -1, 1, 0, 0, 1
    };

    f32* vertices = vec_init(20, sizeof(f32));
    vec_append(&vertices, ve, 20);

    u32* indices = vec_init(6, sizeof(u32));
    vec_append(&indices, in, 6);
    
    Model model = BuildModel(&vertices, &indices, STATIC_DRAW);
    Model gridModel = BuildModel(&vertices, &indices, STATIC_DRAW);
    //cause crashes
    // Entity* trail = Spawn(&scene);
    //  trail->name =  "ballTrail";
    // printf("Trail start\n");
    // trail->transform.position = InitVector3(app.windowWidth * 0.5f, app.windowHeight * 0.5f, 0.1f);
    // trail->transform.position.z += 0.01f;


    // trail->transform.scale = InitVector3(50.0f, 50.0f, 1.0f);
    // trail->image = &circleImage;
    // trail->model = &model;
    // trail->shaderId = shaderProgram;
    // trail->color = InitVector4(1.0f, 1.0f, 1.0f, 0.5f);
    // trail->data = calloc(1, sizeof(Trail));
    // ((Trail*)trail->data)->life = 10.0f;
    // trail->Update = TrailUpdate;
    // trail->Draw = TrailDraw;
    // trail->OnDestroy = TrailOnDestroy;
    Entity* ball = Spawn(&scene);
    
    ball->transform.position = InitVector3(app.windowWidth * 0.5f, app.windowHeight * 0.5f, 0.1f);
    ball->data = calloc(1, sizeof(Ball));
    ball->name = "ball";
    ball->image = &circleImage;
    ball->model = &model;
    ball->shaderId = shaderProgram;
    ball->Start = BallStart;
    ball->Update = BallUpdate;
    ball->Draw = BallDraw;
    ball->OnDestroy = BallOnDestroy;
    ((Ball*)ball->data)->trailTimer = 0.f;

    Entity* leftPaddle = Spawn(&scene);
    leftPaddle->name = "leftPaddle";
    leftPaddle->transform.position = InitVector3(16.0f, app.windowHeight * 0.5f, 0.1f);
    leftPaddle->data = calloc(1, sizeof(Paddle));
    leftPaddle->image = &squareImage;
    leftPaddle->model = &model;
    leftPaddle->shaderId = shaderProgram;
    leftPaddle->color = InitVector4(1.0f, 0.0f, 0.0f, 1.0f);
    leftPaddle->Start = PaddleStart;
    leftPaddle->Update = PaddleUpdate;
    leftPaddle->Draw = PaddleDraw;
    leftPaddle->OnDestroy = PaddleOnDestroy;
    

    Entity* rightPaddle = Spawn(&scene);
    rightPaddle->name = "rightPaddle";
    rightPaddle->transform.position = InitVector3(app.windowWidth - 16.0f, app.windowHeight * 0.5f, 0.1f);
    rightPaddle->data = calloc(1, sizeof(Paddle));
    rightPaddle->image = &squareImage;
    rightPaddle->model = &model;
    rightPaddle->shaderId = shaderProgram;
    rightPaddle->color = InitVector4(0.0f, 0.0f, 1.0f, 1.0f);
    rightPaddle->Start = PaddleStart;
    rightPaddle->Update = PaddleUpdate;
    rightPaddle->Draw = PaddleDraw;
    rightPaddle->OnDestroy = PaddleOnDestroy;
    
    SceneStart(&app, &scene);
    
    bool running = true;
    f32 time = 0.0f;
    while(running) {
        
        InputManagerNewFrame(&app);
        

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // render
        ClearWindow();

        app.projection = Mat4Orthographic(0.0f, (float)app.windowWidth, 0.0f, (float)app.windowHeight,0.001f, 100.0f);
        app.view = IdentityMatrix4();
        Mat4Translate(&app.view, InitVector3(0.0f, 0.0f, -5.0f));
        static Vector2 gridOffset = {0.0f,0.0f};
        const float gridSpeed = 20.0f;
        gridOffset.x += gridSpeed * app.deltaTime;
        gridOffset.y += gridSpeed * app.deltaTime;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         BindShader(gridShader);
        
         ShaderSetMatrix4(gridShader, "VIEW", app.view);
         ShaderSetMatrix4(gridShader, "PROJECTION", app.projection);
        
        Matrix4 transform = IdentityMatrix4();
         Mat4Scale(&transform, InitVector3(2*app.windowWidth,2*app.windowHeight, 1.0f));
         ShaderSetMatrix4(gridShader, "TRANSFORM", transform);
        
         ShaderSetFloat(gridShader, "u_spacing", 20.0f);
         ShaderSetVector4(gridShader, "u_resolution", InitVector4(app.windowWidth, app.windowHeight, 0.0f, 0.0f)); 
        
         ShaderSetVector4(gridShader, "u_gridDark", InitVector4(0.1f, 0.5f, 0.5f, 1.0f));
         ShaderSetVector4(gridShader, "u_gridLight", InitVector4(0.5f, 0.1f, 1.0f, 1.0f));
         ShaderSetVector2(gridShader, "u_offset", gridOffset);
       

         DrawModel(gridModel);
        
        // BindShader(noiseShader);
        // Mat4Translate(&app.view, InitVector3(0.0f, 0.0f, -4.0f));
        // ShaderSetMatrix4(noiseShader, "VIEW", app.view);
        // ShaderSetMatrix4(noiseShader, "PROJECTION", app.projection);
        // Matrix4 transform = IdentityMatrix4();
        // Mat4Scale(&transform, InitVector3(2*app.windowWidth,2*app.windowHeight, 1.0f));
        // ShaderSetMatrix4(noiseShader, "TRANSFORM", transform);
        // ShaderSetVector2(noiseShader, "u_resolution", InitVector2(app.windowWidth, app.windowHeight)); 
        // ShaderSetFloat(noiseShader, "u_time", app.time);
        // DrawModel(gridModel);

        if (app.time != 0.0f)
            app.deltaTime = (SDL_GetTicksNS() * 1e-9) -  app.time;
        
        app.time = SDL_GetTicksNS() * 1e-9;


        app.projection = Mat4Orthographic(0.0f, (float)app.windowWidth, 0.0f, (float)app.windowHeight, 0.001f, 100.0f); 
        app.view = IdentityMatrix4(); 
        Mat4Translate(&app.view, InitVector3(0.0f, 0.0f, -0.5f));

        SceneUpdate(&app, &scene);
        
        

        SceneDraw(&app, &scene);

        SwapWindow(&app);
    }

    FreeModel(model);

    free(iconImage.data);
    free(containerImage.data);
    free(circleImage.data);
    free(squareImage.data);

    SceneFree(&scene);

    FreeWindow(&app);

    DeleteShader(shaderProgram);
    DeleteShader(gridShader);
    DeleteShader(noiseShader);
    return 0;
}
