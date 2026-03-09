#pragma once
#include "cpup/canis.h"
#include "cpup/scene.h"
#include "cpup/model.h"
#include "cpup/vec.h"
#include "cpup/math.h"
#include "cpup/inputmanager.h"
#include "ball.h"



typedef struct {
    int var;
} Paddle;

void PaddleStart(AppContext* _app, Entity* _entity) {
    _entity->transform.rotation = 0.0f;
    _entity->transform.scale = InitVector3(32.0f, 128.0f, 1.0f);
}

void PaddleUpdate(AppContext* _app, Entity* _entity) {
 float speed = 400.f;
    if (strcmp(_entity->name, "leftPaddle") == 0){
        if(GetKey(_app, SDL_SCANCODE_S)){
            _entity->transform.position.y -= speed * _app->deltaTime;
        }if(GetKey(_app, SDL_SCANCODE_W)){
            _entity->transform.position.y += speed * _app->deltaTime;
        }
        }
    else if(strcmp(_entity->name, "rightPaddle") == 0){
        if (GetKey(_app, SDL_SCANCODE_DOWN)){
                _entity->transform.position.y -= speed * _app -> deltaTime;
        }
        if (GetKey(_app, SDL_SCANCODE_UP)){
                _entity->transform.position.y += speed * _app -> deltaTime;
        }
    }
    float inScreen = _entity->transform.scale.y *0.5f;

    if(_entity->transform.position.y < inScreen)
        {
        _entity->transform.position.y = inScreen;
        }
    if(_entity->transform.position.y > _app->windowHeight - inScreen){
            _entity->transform.position.y = _app->windowHeight - inScreen;    
    }
}
 


void PaddleDraw(AppContext* _app, Entity* _entity) {
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
    glUseProgram(_entity->shaderId);
    DrawModel(*_entity->model);

    UnBindShader();
}

void PaddleOnDestroy(AppContext* _app, Entity* _entity) {

}