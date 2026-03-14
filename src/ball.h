#pragma once
#include "cpup/canis.h"
#include "cpup/scene.h"
#include "cpup/model.h"
#include "cpup/inputmanager.h"
#include "paddle.h"
#include "trail.h"



#include <SDL3/SDL.h>

typedef struct {
    int leftScore;
    int rightScore;
    float trailTimer;
    bool trailSpawned;
    bool gameOver;
} Ball;




Entity* SpawnBall(AppContext* _app, Entity* _entity);



bool CheckCollision(Entity* a, Entity* b){
    float ax = a->transform.position.x;
    float ay = a->transform.position.y;
    float aw = a->transform.scale.x;
    float ah = a->transform.scale.y;

    float bx = b->transform.position.x;
    float by = b->transform.position.y;
    float bw = b->transform.scale.x;
    float bh = b->transform.scale.y;

    if(fabs(ax - bx) < (aw + bw) * 0.5f && fabs(ay - by) < (ah + bh) * 0.5f)
    {
        return true;
    }
    return false;
}
void BallDraw(AppContext* _app, Entity* _entity) {
    Matrix4 transform = IdentityMatrix4(); // the order is important
    Mat4Translate(&transform, _entity->transform.position);
    Mat4Rotate(&transform, _entity->transform.rotation * DEG2RAD, InitVector3(0.0f, 0.0f, 1.0f));
    Mat4Scale(&transform, InitVector3(_entity->transform.scale.x, _entity->transform.scale.y, _entity->transform.scale.z));

    BindShader(_entity->shaderId);

    ShaderSetFloat(_entity->shaderId, "TIME", _app->time);
    ShaderSetMatrix4(_entity->shaderId, "VIEW", _app->view);
    ShaderSetMatrix4(_entity->shaderId, "PROJECTION", _app->projection);

    ShaderSetVector4(_entity->shaderId, "COLOR", _entity->color);
    ShaderBindTexture(_entity->shaderId, _entity->image->id, "MAIN_TEXTURE", 0);
    ShaderSetMatrix4(_entity->shaderId, "TRANSFORM", transform);
    DrawModel(*_entity->model);

    UnBindShader();
}
void BallStart(AppContext* _app, Entity* _entity) {
    _entity->color = InitVector4(1.0f, 1.0f, 1.0f, 1.0f);

    _entity->transform.scale = InitVector3(32.0f, 32.0f, 0.5f);

    
    
}
    void vallsUpdate(AppContext* _app, Entity* _entity){
        Vector3 delta = Vec2ToVec3(Vec2Mul(_entity->velocity,_app->deltaTime));
        _entity->transform.position = Vec3Add(_entity->transform.position, delta);

    }

  void Valls(AppContext* _app, Entity* _entity){//works but doesnt spawn anything in
      Ball* ball = (Ball*)(_entity->data);
          for(int i = 0; i < 30; i++){
              Entity* ball = Spawn(&_app->scene);
              float x = rand() % _app->windowWidth;
              float y = rand() % _app->windowHeight + (rand() % 100);
              ball->transform.position = InitVector3(x,y, 1.5f);
              ball->transform.scale = InitVector3(16.0f, 16.0f, 1.0f);
              ball->velocity = InitVector2((rand() % 20 - 10), -(50 + rand() % 50));
              ball->image = _entity->image;
              ball->model = _entity->model;
              ball->shaderId = _entity->shaderId;
              ball->color = _entity->color;
              ball->Draw = BallDraw;
              ball->Update = vallsUpdate;
              }
             };
         
     
 
void UpdateWindowTitle(AppContext* _app, Ball* ball, Entity* _entity) {

char title[128];

if(ball->leftScore >= 5){
    ball->gameOver = true;
    snprintf(title, sizeof(title), "left Player wins!");
    Valls(_app, _entity);
}else if(ball->rightScore >= 5){
    ball->gameOver = true;
    snprintf(title, sizeof(title), "Right Player Wins!");
    Valls(_app, _entity);
}else{
    snprintf(title, sizeof(title), "left: %d | Right: %d", ball->leftScore, ball->rightScore);
}
SDL_SetWindowTitle(_app->window, title);
}
void BallUpdate(AppContext* _app, Entity* _entity) {


    if (GetKey(_app, SDL_SCANCODE_P))
    {
        SpawnBall(_app, _entity);
        
    }

    if (Vec2EqualsZero(_entity->velocity) && GetKeyDown(_app, SDL_SCANCODE_SPACE))
    {
        i32 startingDirection = rand() % 4;

        static Vector2 directions[4] = {
            (Vector2){0.72f, 0.72f},
            (Vector2){0.72f, -0.72f},
            (Vector2){-0.72f, 0.72f},
            (Vector2){-0.72f, -0.72f},
    
        };

        _entity->velocity = Vec2Mul(directions[startingDirection], 150.0f);
        
    } 
    

    // check if ball is heading below the screen
    
    Entity* lp = Find(&_app->scene, "leftPaddle");
    Entity* rp = Find(&_app->scene, "rightPaddle");

    if(lp && CheckCollision(_entity, lp) && _entity->velocity.x < 0){
        _entity->velocity.x *= -1;
        _entity->velocity.x *= 1.2f;
        _entity->color = lp->color;
        float offset = _entity->transform.position.y - lp->transform.position.y;
        _entity->velocity.y = offset * 5.0f;
        float paddleEdge = lp->transform.position.x + lp->transform.scale.x * 0.5f;
        _entity->transform.position.x = paddleEdge + _entity->transform.scale.x * 0.5f;
        OnBallHitPaddle(lp, _entity);
    }
    else if(rp && CheckCollision(_entity, rp) && _entity->velocity.x > 0){
        _entity->velocity.x *= -1;
        _entity->velocity.x *= 1.2f;
        _entity->color = rp->color;

        float offset = _entity->transform.position.y - rp->transform.position.y;
        _entity->velocity.y = offset * 5.0f;
        float paddleEdge = rp->transform.position.x - rp->transform.scale.x * 0.5f;
        _entity->transform.position.x = paddleEdge - _entity->transform.scale.x * 0.5f;
        OnBallHitPaddle(rp, _entity);
    }
    if (_entity->transform.position.y - _entity->transform.scale.y * 0.5f <= 0.0f && _entity->velocity.y < 0.0f)
        _entity->velocity.y *= -1.0f; 
    
    // check if ball is heading above the screen
    if (_entity->transform.position.y + _entity->transform.scale.y * 0.5f >= _app->windowHeight && _entity->velocity.y > 0.0f){
        _entity->velocity.y *= -1.0f; 
    }
    Vector3 delta = Vec2ToVec3(Vec2Mul(_entity->velocity, _app->deltaTime));
    _entity->transform.position = Vec3Add(_entity->transform.position, delta);

    Ball* ball = (Ball*)(_entity->data);
    if (_entity->transform.position.x < 0.5f)
    {
            ball->rightScore++;
            _entity->transform.position = InitVector3(_app->windowWidth * 0.5f, _app->windowHeight * 0.5f, 0.0f);
            _entity->velocity = InitVector2(0.0f, 0.0f);
             
            }
    

    
                


    
    else if (_entity->transform.position.x > _app->windowWidth){
            
            ball->leftScore++;
            
            _entity->transform.position = InitVector3(_app->windowWidth * 0.5f, _app->windowHeight * 0.5f, 0.0f);
            _entity->velocity = InitVector2(0.0f, 0.0f);
            
    }
    if(!Vec2EqualsZero(_entity->velocity) && !ball->trailSpawned){
        ball->trailTimer += _app->deltaTime;
        if(ball->trailTimer > 0.05f){
            //SpawnTrail(_app, _entity);
            ball->trailTimer = 0.0f;
        }
    }
    UpdateWindowTitle(_app, ball, _entity);
    if(ball->gameOver){
        _entity->velocity = InitVector2(0.0f, 0.0f);
        
        return;
    }
}






void BallOnDestroy(AppContext* _app, Entity* _entity) {

}

Entity* SpawnBall(AppContext* _app, Entity* _entity) {
    Scene** scene = &(_app->scene);
    Entity* ball = Spawn(scene);
    ball->transform.position = InitVector3(_app->windowWidth * 0.5f, _app->windowHeight * 0.5f, 0.0f);
    ball->data = calloc(1, sizeof(Ball));
    
    ball->image = _entity->image;
    ball->model = _entity->model;
    ball->shaderId = _entity->shaderId;
    ball->Start = BallStart;
    ball->Update = BallUpdate;
    ball->Draw = BallDraw;
    ball->OnDestroy = BallOnDestroy;
    return ball;
}
