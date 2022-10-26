#include "CustomPhysic.h"
#include "engine\Transform.h"
#include "CustomObject.h"
#include "Box2D\Dynamics\b2Body.h"

CustomPhysics::CustomPhysics()
{}

CustomPhysics::~CustomPhysics()
{}

/**
* \brief
* Check either if the physics component has body
*
* \return m_allocatedBody
*/
bool CustomPhysics::HasBody() const
{
    return m_allocatedBody;
}

/**
* \brief Decallocate the body
* *This does not erase the iterator
* from physics list of State class
*/
void CustomPhysics::Free()
{
    // Check either if physics is allocated before
    if (m_body) {
        // Deregister rigid body from the physics world
        m_world->DestroyBody(m_body);
        // Set pointer to null
        m_body = nullptr;
        m_allocatedBody = false;
    }
}

/**
* \brief
* Set velocitiy of rigid body with two floats
*
* \param x
* X component of velocitiy
*
* \param y
* Y component of velocitiy
*/
void CustomPhysics::SetVelocity(float x, float y)
{
    if (m_body) {
        b2Vec2 velocity(x, y);
        m_body->SetLinearVelocity(velocity);
    }
}

/**
* \brief
* Set velocitiy of rigid body with one vector
*
* \param velocity
* Vector2 for velocity
*/
void CustomPhysics::SetVelocity(const b2Vec2& velocity)
{
    if (m_body)
        m_body->SetLinearVelocity(velocity);
}

/**
* \brief
* Return velocity as vector2
*
* \return m_velocity
*/
const b2Vec2& CustomPhysics::GetVelocity() const
{
    return m_body->GetLinearVelocity();
}

/**
* \brief
* Set density of rigid body
*
* \param density
* float type value for density
*/
void CustomPhysics::SetDensity(float density)
{
    if (m_body)
        m_body->GetFixtureList()->SetDensity(density);
    else
        m_fixtureDef.density = density;
}

/**
* \brief
* Return density as float
*
* \return m_fixtureDef.density
*/
float CustomPhysics::GetDensity()
{
    return m_body->GetFixtureList()->GetDensity();
}

/**
* \brief
* Set restitution of rigid body
*
* \param restitution
* float type value for restitution
*/
void CustomPhysics::SetRestitution(float restitution)
{
    if (m_body)
        m_body->GetFixtureList()->SetRestitution(restitution);
    else
        m_fixtureDef.restitution = restitution;
}

/**
* \brief
* Return restitution as float
*
* \return m_fixtureDef.restitution
*/
float CustomPhysics::GetRestitution()
{
    return m_body->GetFixtureList()->GetRestitution();
}

/**
* \brief
* Set active toggle of physics component
* so box2d world either updates this component or not
*
* \param toggle
* Set true to activate or false to inactivate
*/
void CustomPhysics::ActivePhysics(bool toggle)
{
    if (m_body) {
        m_active = toggle;
        m_body->SetActive(m_active);
    }
}

/**
* \brief
* Check either if physics component is activated or not
*
* \return m_active
*/
bool CustomPhysics::IsActive() const
{
    return m_active;
}

/**
* \brief
* Check either if the body is colliding
* with specific object(body)
*
* \param other
* The object to check with
*
* \return
* True if the body is colliding with 'other'
* False if the body is not colliding with 'other'
*/
bool CustomPhysics::IsCollidingWith(CustomBaseObject* other) const
{
    // Check valid pointer and
    // either if it's comparing with its own
    if (other
        && other != m_pOwner
        && m_activeCollision
        && other->customPhysics.m_activeCollision) {
        b2ContactEdge* contactList = m_body->GetContactList();
        if (contactList) {

            b2Contact* contact = contactList->contact;
            if (contact &&
                contactList->other == other->customPhysics.m_body)
                return contact->IsTouching();

            return false;
        }
        return false;
    }
    return false;
}

/**
* \brief
* Get object colliding with currently
*
* \return
* The pointer to the object if there is,
* unless null
*/
CustomBaseObject* CustomPhysics::GetCollidingWith()
{
    b2ContactEdge* contactList = m_body->GetContactList();
    if (contactList
        && m_activeCollision) {
        b2Body* other = contactList->other;
        b2Contact* contact = contactList->contact;

        for (auto otherBody : *m_pList) {
            if (contact
                && contact->IsTouching()
                && other == otherBody->m_body
                && otherBody->m_pOwner->customPhysics.m_activeCollision)
                return otherBody->m_pOwner;
        }
        return nullptr;
    }
    return nullptr;
}

/**
* \brief
* Check either if the body is colliding
*
* \return
* True if the body is colliding
* False if the body is not colliding
*/
bool CustomPhysics::IsColliding() const
{
    b2ContactEdge* contactList = m_body->GetContactList();
    if (contactList
        && m_activeCollision) {

        b2Contact* contact = contactList->contact;
        if (contact)
            return contact->IsTouching();

        return false;
    }

    return false;
}

/**
* \brief
* Add force to rigid body with two floats
*
* \param x
* Magnitute to x direction
*
* \param y
* Magnitute to y direction
*/
void CustomPhysics::AddForce(float x, float y)
{
    if (m_body)
    {
        b2Vec2 force(x, y);
        b2Vec2 point(pOwnerTransform->position.x / PIXELS_TO_METER, pOwnerTransform->position.y / PIXELS_TO_METER);
        m_body->ApplyLinearImpulse(force, point, m_active);
    }
}

/**
* \brief
* Add force to rigid body with vector2
*
* \param force
*/
void CustomPhysics::AddForce(const b2Vec2 force)
{
    if (m_body) {
        b2Vec2 point(pOwnerTransform->position.x / PIXELS_TO_METER, pOwnerTransform->position.y / PIXELS_TO_METER);
        m_body->ApplyLinearImpulse(force, point, m_active);
    }
}

/**
* \brief
* Set toggle of collision
*
* \param active
* Set true to activate or false to inactivate
*/
void CustomPhysics::ActiveCollision(bool active)
{
    if (m_body) {
        m_activeCollision = active;
        m_body->GetFixtureList()->SetSensor(!m_activeCollision);
    }
}

/**
* \brief
* Check either if collision is activated or not
*
* \return m_activeCollision
*/
bool CustomPhysics::IsActiveCollision() const
{
    return m_activeCollision;
}

/**
* \brief
* Make body check collision but not respond
*
* \param active
*/
void CustomPhysics::ActiveGhostCollision(bool active)
{
    if (m_body) {
        m_isGhost = active;
        m_body->GetFixtureList()->SetSensor(!m_isGhost);
    }
}

/**
* \brief
* Check either if the ghost collision is activated
*
* \return m_isGhost
*/
bool CustomPhysics::IsActiveGhost() const
{
    return m_isGhost;
}

/**
* \brief
* Allocate box2d rigid body to object
* from box2d world which manages every physics component
*
* \param world
* Address of box2d world
*
* \param transform
* Address of object's transform
*/
void CustomPhysics::GenerateBody(b2World* world, Transform* transform)
{
    // Make sure potiners to world and transform are c
    if (world && transform && !m_body) {

        m_world = world;		// Get pointer to world
        pOwnerTransform = transform;	// Connect the transform

                                                                        // Set the starting position
        m_bodyDef.position.Set(
            pOwnerTransform->position.x / PIXELS_TO_METER,
            pOwnerTransform->position.y / PIXELS_TO_METER);

        // Set the starting angle
        m_bodyDef.angle = pOwnerTransform->rotation;

        // Set box info
        if (bodyShape == BOX) {
            m_bodyBox.SetAsBox(
                pOwnerTransform->GetScale().x * .5f / PIXELS_TO_METER,
                pOwnerTransform->GetScale().y * .5f / PIXELS_TO_METER);
            m_fixtureDef.shape = &(m_bodyBox);
        }
        // Set circle info
        else if (bodyShape == CIRCLE) {
            m_bodyBall.m_radius = radius / PIXELS_TO_METER;
            m_fixtureDef.shape = &(m_bodyBall);
        }

        // Set body type
        if (bodyType == DYNAMIC)
            m_bodyDef.type = b2_dynamicBody;
        else if (bodyType == STATIC)
            m_bodyDef.type = b2_staticBody;

        // Create a body and allocate to the pointer to body of physics component
        m_body = m_world->CreateBody(&(m_bodyDef));
        m_body->CreateFixture(&(m_fixtureDef));
        m_allocatedBody = true;
    }
}

/**
* \brief
* Get pointer to rigidbody
*
* \return m_body
*/
b2Body* CustomPhysics::GetBody()
{
    return m_body;
}

/**
* \brief
* Allocate box2d rigid body to object
* from box2d world which manages every physics component
* TODO: This function is deprecated.
* Use GenerateBody(b2World* world, Transform* transform) function instead!
*
* \param world
* Address of box2d world
*/
void CustomPhysics::AllocateBody(b2World* world)
{
    if (!m_body && world) {

        // Get pointer to world
        m_world = world;

        // Set the starting position
        m_bodyDef.position.Set(
            pOwnerTransform->position.x / PIXELS_TO_METER,
            pOwnerTransform->position.y / PIXELS_TO_METER);

        // Set the starting angle
        m_bodyDef.angle = pOwnerTransform->rotation;

        // Set box info
        if (bodyShape == BOX) {
            m_bodyBox.SetAsBox(
                pOwnerTransform->GetScale().x * .5f / PIXELS_TO_METER,
                pOwnerTransform->GetScale().y * .5f / PIXELS_TO_METER);
            m_fixtureDef.shape = &(m_bodyBox);
        }
        // Set circle info
        else if (bodyShape == CIRCLE) {
            m_bodyBall.m_radius = radius / PIXELS_TO_METER;
            m_fixtureDef.shape = &(m_bodyBall);
        }

        // Set body type
        if (bodyType == DYNAMIC)
            m_bodyDef.type = b2_dynamicBody;
        else if (bodyType == STATIC)
            m_bodyDef.type = b2_staticBody;

        // Create a body and allocate to the pointer to body of physics component
        m_body = m_world->CreateBody(&m_bodyDef);
        m_body->CreateFixture(&m_fixtureDef);

        m_allocatedBody = true;
    }
}