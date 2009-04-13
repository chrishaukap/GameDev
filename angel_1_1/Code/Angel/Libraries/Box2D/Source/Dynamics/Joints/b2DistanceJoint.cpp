/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "b2DistanceJoint.h"
#include "../b2Body.h"
#include "../b2World.h"

// C = norm(p2 - p1) - L
// u = (p2 - p1) / norm(p2 - p1)
// Cdot = dot(u, v2 + cross(w2, r2) - v1 - cross(w1, r1))
// J = [-u -cross(r1, u) u cross(r2, u)]
// K = J * invM * JT
//   = invMass1 + invI1 * cross(r1, u)^2 + invMass2 + invI2 * cross(r2, u)^2

void b2DistanceJointDef::Initialize(b2Body* b1, b2Body* b2,
									const b2Vec2& anchor1, const b2Vec2& anchor2)
{
	body1 = b1;
	body2 = b2;
	localAnchor1 = body1->GetLocalPoint(anchor1);
	localAnchor2 = body2->GetLocalPoint(anchor2);
	b2Vec2 d = anchor2 - anchor1;
	length = d.Length();
}


b2DistanceJoint::b2DistanceJoint(const b2DistanceJointDef* def)
: b2Joint(def)
{
	m_localAnchor1 = def->localAnchor1;
	m_localAnchor2 = def->localAnchor2;
	m_length = def->length;
	m_force = 0.0f;
}

void b2DistanceJoint::InitVelocityConstraints(const b2TimeStep& step)
{
	b2Body* b1 = m_body1;
	b2Body* b2 = m_body2;

	// Compute the effective mass matrix.
	b2Vec2 r1 = b2Mul(b1->m_xf.R, m_localAnchor1 - b1->GetLocalCenter());
	b2Vec2 r2 = b2Mul(b2->m_xf.R, m_localAnchor2 - b2->GetLocalCenter());
	m_u = b2->m_sweep.c + r2 - b1->m_sweep.c - r1;

	// Handle singularity.
	float32 length = m_u.Length();
	if (length > b2_linearSlop)
	{
		m_u *= 1.0f / length;
	}
	else
	{
		m_u.Set(0.0f, 0.0f);
	}

	float32 cr1u = b2Cross(r1, m_u);
	float32 cr2u = b2Cross(r2, m_u);
	m_mass = b1->m_invMass + b1->m_invI * cr1u * cr1u + b2->m_invMass + b2->m_invI * cr2u * cr2u;
	b2Assert(m_mass > FLT_EPSILON);
	m_mass = 1.0f / m_mass;

	if (b2World::s_enableWarmStarting)
	{
		b2Vec2 P = step.dt * m_force * m_u;
		b1->m_linearVelocity -= b1->m_invMass * P;
		b1->m_angularVelocity -= b1->m_invI * b2Cross(r1, P);
		b2->m_linearVelocity += b2->m_invMass * P;
		b2->m_angularVelocity += b2->m_invI * b2Cross(r2, P);
	}
	else
	{
		m_force = 0.0f;
	}
}

void b2DistanceJoint::SolveVelocityConstraints(const b2TimeStep& step)
{
	b2Body* b1 = m_body1;
	b2Body* b2 = m_body2;

	b2Vec2 r1 = b2Mul(b1->m_xf.R, m_localAnchor1 - b1->GetLocalCenter());
	b2Vec2 r2 = b2Mul(b2->m_xf.R, m_localAnchor2 - b2->GetLocalCenter());

	// Cdot = dot(u, v + cross(w, r))
	b2Vec2 v1 = b1->m_linearVelocity + b2Cross(b1->m_angularVelocity, r1);
	b2Vec2 v2 = b2->m_linearVelocity + b2Cross(b2->m_angularVelocity, r2);
	float32 Cdot = b2Dot(m_u, v2 - v1);
	float32 force = -step.inv_dt * m_mass * Cdot;
	m_force += force;

	b2Vec2 P = step.dt * force * m_u;
	b1->m_linearVelocity -= b1->m_invMass * P;
	b1->m_angularVelocity -= b1->m_invI * b2Cross(r1, P);
	b2->m_linearVelocity += b2->m_invMass * P;
	b2->m_angularVelocity += b2->m_invI * b2Cross(r2, P);
}

bool b2DistanceJoint::SolvePositionConstraints()
{
	b2Body* b1 = m_body1;
	b2Body* b2 = m_body2;

	b2Vec2 r1 = b2Mul(b1->m_xf.R, m_localAnchor1 - b1->GetLocalCenter());
	b2Vec2 r2 = b2Mul(b2->m_xf.R, m_localAnchor2 - b2->GetLocalCenter());

	b2Vec2 d = b2->m_sweep.c + r2 - b1->m_sweep.c - r1;

	float32 length = d.Normalize();
	float32 C = length - m_length;
	C = b2Clamp(C, -b2_maxLinearCorrection, b2_maxLinearCorrection);

	float32 impulse = -m_mass * C;
	m_u = d;
	b2Vec2 P = impulse * m_u;

	b1->m_sweep.c -= b1->m_invMass * P;
	b1->m_sweep.a -= b1->m_invI * b2Cross(r1, P);
	b2->m_sweep.c += b2->m_invMass * P;
	b2->m_sweep.a += b2->m_invI * b2Cross(r2, P);

	b1->SynchronizeTransform();
	b2->SynchronizeTransform();

	return b2Abs(C) < b2_linearSlop;
}

b2Vec2 b2DistanceJoint::GetAnchor1() const
{
	return m_body1->GetWorldPoint(m_localAnchor1);
}

b2Vec2 b2DistanceJoint::GetAnchor2() const
{
	return m_body2->GetWorldPoint(m_localAnchor2);
}

b2Vec2 b2DistanceJoint::GetReactionForce() const
{
	b2Vec2 F = m_force * m_u;
	return F;
}

float32 b2DistanceJoint::GetReactionTorque() const
{
	return 0.0f;
}
