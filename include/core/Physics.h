#pragma once

#include "core/PhysicsComponent.h"
#include <array>
#include <cmath>
#include <string>
#include <vector>

#ifdef _WIN32
	#undef max
	#undef min
#endif

namespace age
{
	struct CollisionPair
	{
		int A;
		int B;

		CollisionPair(int first, int second) : A(first), B(second) {};

		bool operator==(const CollisionPair& other)
		{
			if (other.A == A && other.B == B)
			{
				return true;
			}
			else if (other.B == A && other.A == B)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	struct CollisionInfo
	{
		CollisionInfo() : comp(-1), other(-1), normal(0.0f, 0.0f), penetration(0.0f) {};
		CollisionInfo(int c, int o, Vec2f pt, Vec2f n, float p) : comp(c), other(o), normal(n), penetration(p)
		{
			num_points = 1;
			closestPtWorld[0] = pt;
		};
		CollisionInfo(int c, int o, Vec2f pt1, Vec2f pt2, Vec2f n, float p)
			: comp(c), other(o), normal(n), penetration(p)
		{
			num_points = 2;
			closestPtWorld[0] = pt1;
			closestPtWorld[1] = pt2;
		};

		int comp;
		int other;
		int num_points = 0;
		Vec2f closestPtWorld[2];
		Vec2f normal;
		float penetration;
	};

	struct CollisionResult
	{
		CollisionResult() : colliding(false), info() {};
		CollisionResult(int c, int o, Vec2f pt, Vec2f n, float p) : colliding(true), info(c, o, pt, n, p) {};
		CollisionResult(int c, int o, Vec2f pt1, Vec2f pt2, Vec2f n, float p)
			: colliding(true), info(c, o, pt1, pt2, n, p) {};
		bool colliding = false;
		CollisionInfo info;
	};

	struct CollisionImpulses
	{
		Vec2f linearA;
		Vec2f linearB;
		float angularA;
		float angularB;
	};

	class Physics
	{
	  public:
		void ResolveCollision(PhysicsComponent* array, CollisionInfo& c)
		{
			PhysicsComponent& component = array[c.comp];
			PhysicsComponent& other = array[c.other];

			CollisionImpulses result[2];

			for (int i = 0; i < c.num_points; ++i)
			{
				Vec2f& point = c.closestPtWorld[i];

				Vec2f raPerp = (point - component.pos()).perp();
				Vec2f rbPerp = (point - other.pos()).perp();

				Vec2f angularLinearVelocityA = raPerp * component.angularVelocity;
				Vec2f angularLinearVelocityB = rbPerp * other.angularVelocity;

				Vec2f relativeVelocity = (component.linearVelocity + angularLinearVelocityA) -
										 (other.linearVelocity + angularLinearVelocityB);

				float closingVelocity = relativeVelocity.dot(c.normal);
				// if (closingVelocity > 0.0f)
				//	closingVelocity *= -1.0f;
				// float closingVelocity = -(component.linearVelocity - other.linearVelocity).dot(component.pos() -
				// other.pos());
				float totalInvMass = component.invmass + other.invmass;

				if (totalInvMass > 0)
				{
					float e = 1.0f;
					float num = -(1.0f + e) * closingVelocity;

					// Vec2f n_scaled = c.normal * (component.invmass + other.invmass);
					// float ndotn_scaled = c.normal.dot(n_scaled);

					float a_ri = 0.0f;
					if (component.inertia > 0.0f)
					{
						float r_apdotn = raPerp.dot(c.normal);
						float a_num = r_apdotn * r_apdotn;
						a_ri = a_num == 0.0f ? 0.0f : a_num / component.inertia;
					}

					float b_ri = 0.0f;
					if (other.inertia > 0.0f)
					{
						float r_bpdotn = rbPerp.dot(c.normal);
						float b_num = r_bpdotn * r_bpdotn;
						b_ri = b_num == 0.0f ? 0.0f : b_num / other.inertia;
					}

					// float j = num / (ndotn_scaled + a_ri + b_ri);
					float j = num / ((component.invmass + other.invmass) + a_ri + b_ri);
					j /= c.num_points;

					result[i].linearA = (c.normal * j) * component.invmass;
					result[i].linearB = (c.normal * -j) * other.invmass;

					result[i].angularA = component.inertia > 0.0f ? raPerp.dot(c.normal * j) / component.inertia : 0.0f;
					result[i].angularB = other.inertia > 0.0f ? rbPerp.dot(c.normal * -j) / other.inertia : 0.0f;

					// component.linearVelocity += c.normal * (j * component.invmass);
					// other.linearVelocity += c.normal * (-j * other.invmass);

					//				if (component.inertia > 0.0f)
					//					component.angularVelocity += (point - component.pos()).perp().dot(c.normal * j)
					/// component.inertia; 				if (other.inertia > 0.0f) 					other.angularVelocity += (point -
					//other.pos()).perp().dot(c.normal* -j) / other.inertia;

					///

					//				float max_velocity = 20.0f;
					//				if (component.linearVelocity.mag() > max_velocity)
					//				{
					//					Vec2f unit = component.linearVelocity.normalized();
					//					component.linearVelocity = unit * max_velocity;
					//				}
				}
			}

			for (int i = 0; i < c.num_points; ++i)
			{
				component.linearVelocity += result[i].linearA;
				other.linearVelocity += result[i].linearB;

				component.angularVelocity += result[i].angularA;
				other.angularVelocity += result[i].angularB;
			}

			component.resolved = true;
			other.resolved = true;
		}

		void UpdatePosition(PhysicsComponent* p, const double dt)
		{
			if (p->isStatic)
				return;

			// p->physHistory[p->histIdx].pos = p->pos;
			p->physHistory[p->histIdx].mat = p->mat;
			p->physHistory[p->histIdx].vel = p->linearVelocity;

			if (p->maxRewind == PhysicsComponent::INVALID_IDX)
			{
				p->maxRewind = p->histIdx;
			}

			p->maxForward = p->histIdx;
			p->histIdx = (p->histIdx + 1) % PhysicsComponent::PHYS_HISTORY_SIZE;

			if (p->histIdx == p->maxRewind)
			{
				p->maxRewind = (p->maxRewind + 1) % PhysicsComponent::PHYS_HISTORY_SIZE;
			}

			// if (p->linearVelocity.squareMag() > 0.0f)
			//{
			auto deltaPos = p->linearVelocity * dt;
			p->mat.translate(deltaPos.x, deltaPos.y);

			auto deltaAngle = p->angularVelocity * dt;
			p->mat.rotate(deltaAngle);
			//}

			// p->pos += p->linearVelocity * dt;

			p->CalculateAABB();

			p->resolved = false;
		}

		float closestPointOnAxis(Vec2f a, Vec2f b, Vec2f p)
		{
			return b.dot(p) / b.squareMag();
		}

		Vec2f closestPoint(Vec2f a, Vec2f b, Vec2f p)
		{
			Vec2f ab = b - a;
			Vec2f ap = p - a;
			// float d = ab.dot(ap) / ab.mag();
			float d = ab.dot(ap) / ab.squareMag();

			if (d <= 0.0f)
				return a;
			if (d >= 1.0f)
				return b;
			// return a + ab.normalized() * d;
			return a + ab * d;
		}

		CollisionResult BoxBoxNarrow(PhysicsComponent* array, int box1_idx, int box2_idx)
		{
			PhysicsComponent& box1 = array[box1_idx];
			PhysicsComponent& box2 = array[box2_idx];

			auto boxToPts = [](const PhysicsComponent box) {
				std::array<Vec2f, 4> result;

				float half_width = box.width / 2.0f;
				float half_height = box.height / 2.0f;

				result[0] = {-half_width, -half_height};
				result[1] = {-half_width, +half_height};
				result[2] = {+half_width, +half_height};
				result[3] = {+half_width, -half_height};

				result[0] = box.mat * result[0];
				result[1] = box.mat * result[1];
				result[2] = box.mat * result[2];
				result[3] = box.mat * result[3];

				return result;
			};

			auto b1 = boxToPts(box1);
			auto b2 = boxToPts(box2);

			// SAT

			Vec2f normal = Vec2f::Zero();
			float depth = std::numeric_limits<float>::max();

			auto intersectionTest = [&](const std::array<Vec2f, 4>& boxPts) {
				for (int i = 0; i < 4; ++i)
				{
					auto edge = boxPts[(i + 1) % 4] - boxPts[i];
					auto axis = edge.perp().normalized();

					float min1 = std::numeric_limits<float>::max();
					float min2 = std::numeric_limits<float>::max();
					float max1 = std::numeric_limits<float>::lowest();
					float max2 = std::numeric_limits<float>::lowest();

					for (int j = 0; j < 4; ++j)
					{
						auto p1 = axis.dot(b1[j]);
						if (p1 < min1)
							min1 = p1;
						if (p1 > max1)
							max1 = p1;

						auto p2 = axis.dot(b2[j]);
						if (p2 < min2)
							min2 = p2;
						if (p2 > max2)
							max2 = p2;
					}

					if (max1 <= min2 || max2 <= min1) // no collision
						return false;

					// collision = true;
					float axisDepth = std::min(max1 - min2, max2 - min1);
					if (axisDepth < depth)
					{
						depth = axisDepth;
						normal = axis;
					}
				}
				return true;
			};

			if (!(intersectionTest(b1) && intersectionTest(b2)))
				return CollisionResult();

			Vec2f dir = box2.pos() - box1.pos();
			if (dir.dot(normal) < 0.0f)
				normal = normal * -1.0f;

			if (box1.invmass == 0 || box1.isKinematic)
			{
				box2.mat.translate(normal * depth);
			}
			else if (box2.invmass == 0 || box2.isKinematic)
			{
				box1.mat.translate(normal * -depth);
			}
			else
			{
				box1.mat.translate(normal * -(depth / 2.0f));
				box2.mat.translate(normal * (depth / 2.0f));
			}

			// -------------------------------

			float dist = std::numeric_limits<float>::max();
			// Vec2f norm = {0.0f, 0.0f};
			// Vec2f fp;
			int num_points = 0;
			Vec2f pt1, pt2;
			// Vec2f p0, p1;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					auto c = closestPoint(b1[j], b1[(j + 1) % 4], b2[i]);
					auto thisDist = (c - b2[i]).mag();
					if (thisDist == dist)
					{
						if (!pt1.almostEqual(c))
						{
							pt2 = c;
							num_points = 2;
						}
					}
					else if (thisDist < dist)
					{
						dist = thisDist;
						pt1 = c;
						num_points = 1;
					}
				}

				for (int j = 0; j < 4; ++j)
				{
					auto c = closestPoint(b2[j], b2[(j + 1) % 4], b1[i]);
					auto thisDist = (c - b1[i]).mag();
					if (abs(thisDist - dist) < 0.0005f)
					{
						if (!pt1.almostEqual(c))
						{
							pt2 = c;
							num_points = 2;
						}
					}
					else if (thisDist < dist)
					{
						dist = thisDist;
						pt1 = c;
						num_points = 1;
					}
				}
			}

			if (num_points == 2)
				return CollisionResult(box1_idx, box2_idx, pt1, pt2, normal, dist);
			else
				return CollisionResult(box1_idx, box2_idx, pt1, normal, dist);

			//		if ((fp - fc).dot(normal) < 0.0f)
			//		{
			//			if (norm_on_first)
			//				return CollisionResult(box1_idx, box2_idx, fc, norm, dist);
			//			else
			//				return CollisionResult(box2_idx, box1_idx, fc, norm, dist);
			//		}
			//		return CollisionResult();

			//			auto c1 = closestPoint(b1[0], b1[1], b2[i]);
			//			if ((c1 - b2[i]).squareMag() < dist)
			//				dist = (c1 - b2[i]).squareMag();
			//			auto c2 = closestPoint(b1[1], b1[2], b2[i]);
			//			if ((c2 - b2[i]).squareMag() < dist)
			//				dist = (c2 - b2[i]).squareMag();
			//			auto c3 = closestPoint(b1[2], b1[3], b2[i]);
			//			if ((c3 - b2[i]).squareMag() < dist)
			//				dist = (c3 - b2[i]).squareMag();
			//			auto c4 = closestPoint(b1[3], b1[0], b2[i]);
			//			if ((c4 - b2[i]).squareMag() < dist)
			//				dist = (c4 - b2[i]).squareMag();
			//
			//			auto c5 = closestPoint(b2[0], b2[1], b1[i]);
			//			if ((c5 - b1[i]).squareMag() < dist)
			//				dist = (c1 - b2[i]).squareMag();
			//			auto c6 = closestPoint(b2[1], b2[2], b1[i]);
			//			if ((c6 - b1[i]).squareMag() < dist)
			//				dist = (c2 - b2[i]).squareMag();
			//			auto c7 = closestPoint(b2[2], b2[3], b1[i]);
			//			if ((c7 - b1[i]).squareMag() < dist)
			//				dist = (c3 - b2[i]).squareMag();
			//			auto c8 = closestPoint(b2[3], b2[0], b1[i]);
			//			if ((c8 - b1[i]).squareMag() < dist)
			//				dist = (c4 - b2[i]).squareMag();
			//		}
		}

		CollisionResult BoxSphereNarrow(PhysicsComponent* array, int box_idx, int sphere_idx)
		{
			PhysicsComponent& box = array[box_idx];
			PhysicsComponent& sphere = array[sphere_idx];

			Mat3f inv = box.mat.inverse();
			Mat3f ident = box.mat * inv;

			Vec2f relativeSpherePos = inv * sphere.pos();

			// Vec2f relCenter = sphere.pos() - box.pos();
			Vec2f relCenter = relativeSpherePos;
			Vec2f closestPt = {0.0f, 0.0f};

			float dist = relCenter.x;
			if (dist > box.width / 2.0f)
				dist = box.width / 2.0f;
			else if (dist < -box.width / 2.0f)
				dist = -box.width / 2.0f;
			closestPt.x = dist;

			dist = relCenter.y;
			if (dist > box.height / 2.0f)
				dist = box.height / 2.0f;
			else if (dist < -box.height / 2.0f)
				dist = -box.height / 2.0f;
			closestPt.y = dist;

			dist = (closestPt - relCenter).squareMag();
			if (dist > sphere.radius * sphere.radius)
				return CollisionResult();

			// Vec2f closestPtWorld = closestPt + box.pos();
			Vec2f closestPtWorld = box.mat * closestPt;
			Vec2f normal = (sphere.pos() - closestPtWorld).normalized();

			float penetration = sphere.radius - sqrt(dist);

			return CollisionResult(sphere_idx, box_idx, closestPtWorld, normal, penetration);
		}

		bool BoxBoxCheck(PhysicsComponent* array, int box1_idx, int box2_idx)
		{
			PhysicsComponent& box1 = array[box1_idx];
			PhysicsComponent& box2 = array[box2_idx];

			if (box1.aabb.x + box1.aabb.w > box2.aabb.x && box1.aabb.x < box2.aabb.x + box2.aabb.w)
			{
				if (box1.aabb.y + box1.aabb.h > box2.aabb.y && box1.aabb.y < box2.aabb.y + box2.aabb.h)
				{
					box1.colliding = true;
					box2.colliding = true;
					return true;
				}
			}
			return false;
		}

		bool BoxSphereCheck(PhysicsComponent* array, int box_idx, int sphere_idx)
		{
			PhysicsComponent& box = array[box_idx];
			PhysicsComponent& sphere = array[sphere_idx];

			if (sphere.pos().x + sphere.radius > box.aabb.x && sphere.pos().x - sphere.radius < box.aabb.x + box.aabb.w)
			{
				if (sphere.pos().y + sphere.radius > box.aabb.y &&
					sphere.pos().y - sphere.radius < box.aabb.y + box.aabb.h)
				{
					sphere.colliding = true;
					box.colliding = true;

					return true;
				}
			}
			return false;
		}

		CollisionResult SphereSphereNarrow(PhysicsComponent* array, int first_idx, int second_idx)
		{
			PhysicsComponent& first = array[first_idx];
			PhysicsComponent& second = array[second_idx];

			Vec2f midline = first.pos() - second.pos();
			Vec2f closestPtWorld = first.pos() + (midline * 0.5f);
			float penetration = (first.radius + second.radius) - midline.mag();

			return CollisionResult(first_idx, second_idx, closestPtWorld, midline.normalized(), penetration);
		}

		bool SphereSphereCheck(PhysicsComponent* array, int first_idx, int second_idx)
		{
			PhysicsComponent& first = array[first_idx];
			PhysicsComponent& second = array[second_idx];

			if ((first.pos() - second.pos()).mag() <= first.radius + second.radius)
				return true;
			return false;
		}

		bool CollisionCheck(PhysicsComponent* array, int first, int second)
		{
			PhysicsComponent& a = array[first];
			PhysicsComponent& b = array[second];

			if (a.isStatic && b.isStatic)
				return false;

			if (a.m_type == PhysicsComponent::PhysType::Box)
			{
				if (b.m_type == PhysicsComponent::PhysType::Box)
				{
					return BoxBoxCheck(array, first, second);
				}
				else if (b.m_type == PhysicsComponent::PhysType::Sphere)
				{
					return BoxSphereCheck(array, first, second);
				}
			}
			else if (a.m_type == PhysicsComponent::PhysType::Sphere)
			{
				if (b.m_type == PhysicsComponent::PhysType::Box)
				{
					return BoxSphereCheck(array, second, first);
				}
				else if (b.m_type == PhysicsComponent::PhysType::Sphere)
				{
					return SphereSphereCheck(array, first, second);
				}
			}
			return false;
		}

		CollisionResult NarrowPhase(PhysicsComponent* array, int first, int second)
		{
			PhysicsComponent* a = &array[first];
			PhysicsComponent* b = &array[second];

			if (a->m_type == PhysicsComponent::PhysType::Box)
			{
				if (b->m_type == PhysicsComponent::PhysType::Box)
				{
					return BoxBoxNarrow(array, first, second);
				}
				else if (b->m_type == PhysicsComponent::PhysType::Sphere)
				{
					return BoxSphereNarrow(array, first, second);
				}
			}
			else if (a->m_type == PhysicsComponent::PhysType::Sphere)
			{
				if (b->m_type == PhysicsComponent::PhysType::Box)
				{
					return BoxSphereNarrow(array, second, first);
				}
				else if (b->m_type == PhysicsComponent::PhysType::Sphere)
				{
					return SphereSphereNarrow(array, second, first);
				}
			}
			return CollisionResult();
		}
	};
} // namespace age
