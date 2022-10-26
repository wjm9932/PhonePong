#pragma once
#include "engine/Object.h"
#include "CustomPhysic.h"

class CustomBaseObject : public Object
{

public:

    CustomBaseObject();
    ~CustomBaseObject() override;

    void RemoveCustomPhysicsComp(b2World* world);

    // New way to remove custom physics
    void RemoveCustomPhysicsComponent();

    CustomPhysics customPhysics;

};