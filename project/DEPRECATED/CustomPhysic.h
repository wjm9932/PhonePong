/******************************************************************************/
/*!
\file   CustomPhysics.h
\author Juyong Jeong
\par    email: jeykop14\@gmail.com
\par    GAM150 demo
\date   2018/03/17

Containing object's velocity, direction, gravity.
This component can be editted by students
* This component is not going to be updated anymore.
*/
/******************************************************************************/
#pragma once
#include "Box2D/Box2D.h"
#include <vector>

class Transform;
class b2World;

class CustomPhysics {

    friend class State;				// Let Base State can access its private members
    friend class CustomBaseObject;	// Let Derived class can access its private members

                                                                    // Box2D Info
    b2FixtureDef	m_fixtureDef;
    b2BodyDef		m_bodyDef;
    b2PolygonShape	m_bodyBox;
    b2CircleShape   m_bodyBall;
    b2Body			*m_body = nullptr;
    b2World			*m_world = nullptr;

    // Physics info
    CustomBaseObject	*m_pOwner = nullptr;

    //float   m_density;
    bool	m_active = true, m_activeCollision = true,
        m_allocatedBody = false, m_isGhost = false;

public:

    enum BodyShape { CIRCLE, BOX };
    enum BodyType { STATIC, DYNAMIC };

    std::vector<CustomPhysics*>	*m_pList = nullptr;

    BodyShape   bodyShape = BOX;			// Either box or ball
    BodyType	bodyType = DYNAMIC;			// Either dynamic or static
    Transform	*pOwnerTransform = nullptr;	// Pointer to the physics' owner transform component
    float       radius = 1.f;

    CustomPhysics();
    ~CustomPhysics();

    // Initialize the physics info 
    // TODO: This function is deprecated.
    // Use GenerateBody(b2World* world, Transform* transform) function instead!
    void AllocateBody(b2World* world);
    // Initialize the physics info (new way)
    // Recommend to use this function rather than GenerateBody()
    // *With this function, users do not have to connect 'pTransform = &transform'
    // Initialize the physics info (New method)
    void GenerateBody(b2World* world, Transform* transform);
    // Get pointer to body
    b2Body* GetBody();
    // Check either if the physics component has body
    bool HasBody() const;
    // Decallocate the body
    void Free();
    // Set velocity with floats
    void SetVelocity(float x, float y);
    // Set velocity with vec2
    void SetVelocity(const b2Vec2& velocity);
    // Get velocity as vec2
    const b2Vec2& GetVelocity() const;
    // Set density 
    void SetDensity(float density);
    // Get density
    float GetDensity();
    // Set restitution
    void SetRestitution(float restitution);
    // Get restitution
    float GetRestitution();
    // Set physics toggle (either let it work or not)
    void ActivePhysics(bool toggle);
    // Get physics toggle 
    bool IsActive() const;
    // Check either if the body is colliding 
    // with specific object(body)
    bool IsCollidingWith(CustomBaseObject* other) const;
    // Get the object which is colliding with 'this' body
    CustomBaseObject* GetCollidingWith();
    // Check either if the body is colliding
    bool IsColliding() const;
    // Add force to body with two floats
    void AddForce(float x, float y);
    // Add force to body with vector2
    void AddForce(const b2Vec2 force);
    // Set coliision toggle
    void ActiveCollision(bool active);
    // Get collision toggle
    bool IsActiveCollision() const;
    // Set ghost collision
    void ActiveGhostCollision(bool active);
    // Get ghost toggle
    bool IsActiveGhost() const;

};