// #pragma once
// # include "cpup/canis.h"
// # include "cpup/scene.h"
// # include "cpup/model.h"
// # include "trail.h"


// void TrailUpdate(AppContext* _app, Entity* _entity){
//     Trail* trail = (Trail*)_entity->data;
//     trail->life -= _app->deltaTime;
//     if(trail->life < 0.0f) trail->life = 0.0f;

//     float lifeRatio = trail->life / trail->maxlife;

//     _entity->color.w = trail->color.w * lifeRatio;
//     _entity->transform.scale = InitVector3(trail->scale.x * lifeRatio, trail->scale.y * lifeRatio, trail->scale.z);

    
// }
    
// void TrailOnDestroy(AppContext* _app, Entity* _entity){
//      if(_entity->data){
//          free(_entity->data);
//      }
//  }

// void TrailDraw(AppContext* _app, Entity* _entity){

//      Matrix4 transform = IdentityMatrix4();
//      Mat4Translate(&transform, _entity->transform.position);
//      Mat4Rotate(&transform, _entity->transform.rotation * DEG2RAD, InitVector3(0,0,1.0f));
//      Mat4Scale(&transform, _entity->transform.scale);

//      BindShader(_entity->shaderId);

//      ShaderSetFloat(_entity->shaderId,"TIME",_app->time);
    
//      ShaderSetMatrix4(_entity->shaderId,"VIEW",_app->view);
    
//      ShaderSetMatrix4(_entity->shaderId,"PROJECTION",_app->projection);
    
//      ShaderSetVector4(_entity->shaderId,"COLOR",_entity->color);
//      ShaderBindTexture(_entity->shaderId, _entity->image->id, "MAIN_TEXTURE", 0);
//      ShaderSetMatrix4(_entity->shaderId,"TRANSFORM", transform);
//      DrawModel(*_entity->model);
//      UnBindShader();
//  }

// Entity* SpawnTrail(AppContext* _app, Entity* source){
//     Entity* trail = Spawn(&_app->scene);
//     trail->name = "balltrail";

//     trail->transform.position = source->transform.position;
//     trail->transform.position.z += 0.01f;
//     trail->transform.scale = InitVector3(50.0f, 50.0f, 1.0f);
//     trail->image = source->image;
//     trail->model = source->model;
//     trail->shaderId = source->shaderId;

//     Trail* t = calloc(1, sizeof(Trail));
//     t->life = 1.0f;
//     t->maxlife = 1.0f;
//     t->scale = trail->transform.scale;
//     t->color = InitVector4(1,1,1,0.5f);

//      trail->Update = TrailUpdate;
//      trail->Draw = TrailDraw;
//      trail->OnDestroy = TrailOnDestroy;
//      return trail;

// }