/******************************************************************************/
/*!
\file   Transform.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/14

Containing object's position, degree, scale attributes

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "Box2D\Common\b2Math.h"
#include "MKS.h"

class Transform {

public:

	Transform() {};
	~Transform() {};

	b2Vec3  position;		// Vector3 position 
	float	rotation = 0.f;	// Rotataion as degree(not radian)

	void SetScale(float x, float y)	{ scale.x = x / PIXELS_TO_METER; scale.y = y / PIXELS_TO_METER;	}
	void SetScale(b2Vec2 set) { scale.x = set.x / PIXELS_TO_METER; scale.y = set.y / PIXELS_TO_METER; }

	b2Vec2 GetScale()
	{
	    b2Vec2 result = scale; result.x *= PIXELS_TO_METER;	result.y *= PIXELS_TO_METER; 
	    return result;
	}
    b2Vec2 GetHalfScale()
	{
	    b2Vec2 result = GetScale(); result.x *= 0.5f; result.y *= 0.5f;
	    return result;
	}
private:

	b2Vec2  scale;      // Vector2 scale

	Transform(const Transform&) = delete;
	Transform(Transform&&) = delete;
	Transform& operator=(const Transform&) = delete;
	Transform& operator=(Transform&&) = delete;
};
