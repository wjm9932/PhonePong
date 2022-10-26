/******************************************************************************/
/*!
\file   PhysicsPipeline.cpp
\author Juyong Jeong
\par    email: jeykop14\@gmail.com
\par    GAM150 demo
\date   2018/03/20

Containing the custom physics system.
This system can be editted by students
* This component is not going to be updated anymore.
*/
/******************************************************************************/
#include "engine\State.h"
#include "engine\Object.h"
#include "Box2D\Dynamics\b2World.h"
#include "CustomObject.h"

/**
* \brief
* Return pointer to current custom physics world
*
* \return m_customWorld
*/
b2World* State::GetCustomPhysicsWorld()
{
    return m_customWorld;
}

/**
* \brief
* Allocate and initialize custom physics world
*
* \param gravity
* Gravity of physics world
*/
void State::SetCustomPhysicsWorld(const b2Vec2& gravity)
{
    // Allocate new box2d physics world
    m_customWorld = new b2World(gravity);
}

/**
* \brief
* Update every custom rigid body
*
* \param dt
* Delta time
*/
void State::UpdateCustomPhysics(float dt)
{
    static const float RadianToDegree = float(180.F / M_PI);
    static const float DegreeToRadian = float(M_PI / 180.F);

    static const int32 velocityIterations = 8;   //how strongly to correct velocity
    static const int32 positionIterations = 3;   //how strongly to correct position

     // If there is an allocated physics world
    if (m_customWorld) {

        // Update every physics components
        m_customWorld->Step(dt, velocityIterations, positionIterations);

        /* TODO: Either update custom physics list or maintain your method
         * Still, recommend to change new way.
         * If you stick with old way, you don't need to call
         * AddCustomPhysicsComponent function
         */

         //// Update transform with outputs from box2d world
         //for (auto object : m_objects) {

         //	// Get custom object
         //	CustomBaseObject* c_obj
         //		= dynamic_cast<CustomBaseObject*>(object);

         //	// Only if physics has rigid body and toggle is on
         //	if (c_obj->customPhysics.m_body
         //		&& c_obj->customPhysics.m_active) {

         //		// Pull the updated position
         //		b2Vec2 tempPosition = c_obj->customPhysics.m_body->GetPosition();
         //		c_obj->transform.position.x = tempPosition.x* PIXELS_TO_METER;
         //		c_obj->transform.position.y = tempPosition.y* PIXELS_TO_METER;

         //		// Pull the updated rotation
         //		c_obj->transform.rotation = c_obj->customPhysics.m_body->GetAngle() * RadianToDegree;

         //	}
         //}

         // Update sprite's transform with outputs from box2d world
        for (auto customPhysics : m_customPhysicsList) {

            // Only if physics has rigid body and toggle is on
            if (customPhysics->m_active) {

                Transform* spriteTransform = customPhysics->pOwnerTransform;
                b2Body* body = customPhysics->m_body;

                // Pull the updated position
                b2Vec2 box2dPosition = body->GetPosition();
                body->SetTransform(box2dPosition, spriteTransform->rotation * DegreeToRadian);
                spriteTransform->position.x = box2dPosition.x * PIXELS_TO_METER;
                spriteTransform->position.y = box2dPosition.y * PIXELS_TO_METER;

                // Pull the updated rotation
                spriteTransform->rotation = body->GetAngle() * RadianToDegree;

            }
        }
    }
}

/**
* \brief
* Add custom physics component to a object
*
* \param object
* Object to add custom physics component
*/
void State::AddCustomPhysicsComponent(CustomBaseObject* object)
{
    CustomPhysics* customPhysics = &object->customPhysics;
    customPhysics->m_pOwner = object;
    customPhysics->m_pList = &m_customPhysicsList;

    if (customPhysics->HasBody())
        m_customPhysicsList.push_back(customPhysics);
}

/**
* \brief
* Deallcaote custom physics world of current state
*/
void State::RemoveCustomPhysicsWorld()
{
    ClearCustomPhysicsComponent();

    // Check either if world is allocated before
    if (m_customWorld) {
        // Delete heap memory of physics world  
        delete m_customWorld;
        // Set pointer to null
        m_customWorld = nullptr;
    }
}


/**
 * \brief
 * Clear all the allocated physics components
 */
void State::ClearCustomPhysicsComponent()
{
    // Clear all the custom physic components
    for (auto object : m_objects)
        dynamic_cast<CustomBaseObject*>(object)->RemoveCustomPhysicsComp(m_customWorld);

    // Clear every iterator from the list
    m_customPhysicsList.clear();
}
