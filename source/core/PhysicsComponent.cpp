#include "core/PhysicsComponent.h"
#include "core/Entity.h"
#include "core/Panel.h"
#include "core/Physics.h"
#include "core/Renderer.h"
#include "core/ScreenManager.h"

namespace age
{
	void PhysicsComponent::InitBox(const Params& params)
	{
		m_type = PhysType::Box;

		aabb = {params.xpos - params.width / 2.0f, params.ypos - params.height / 2.0f, params.width, params.height};
		width = params.width;
		height = params.height;
		isStatic = (params.mass == 0.0f);
		isKinematic = (params.mass < 0.0f);
		eid = params.eID;

		linearVelocity = params.linVel;
		angularVelocity = params.angVel;

		mat.identity();
		mat.rotate(toRadians(params.rot));
		mat.translate(params.xpos, params.ypos);

		invmass = params.mass <= 0.0f ? 0.0f : 1.0f / params.mass;
		inertia = ((height * height) + (width * width)) * params.mass * 1.0f / 12.0f;
	}

	void PhysicsComponent::InitSphere(const Params& params)
	{
		m_type = PhysType::Sphere;
		radius = params.width;
		// pos = { xpos, ypos };
		aabb = {params.xpos, params.ypos, radius * 2.0f, radius * 2.0f};
		linearVelocity = params.linVel;
		angularVelocity = 0.0f;

		invmass = 1.0f / params.mass;
		inertia = 0.5f * params.mass * (radius * radius);

		mat.identity();
		mat.translate(params.xpos, params.ypos);
	}

	void PhysicsComponent::Update(Physics* p, const double dt)
	{
		if (rewind)
		{
			StepBack();
		}
		else
		{
			p->UpdatePosition(this, dt);
		}
	}

	void PhysicsComponent::CalculateAABB()
	{
		if (m_type == PhysicsComponent::PhysType::Box)
		{
			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;

			Vec2f p1 = mat * Vec2f{-halfWidth, -halfHeight};
			Vec2f p2 = mat * Vec2f{-halfWidth, +halfHeight};
			Vec2f p3 = mat * Vec2f{+halfWidth, -halfHeight};
			Vec2f p4 = mat * Vec2f{+halfWidth, +halfHeight};

			float minx = fmin(p1.x, fmin(p2.x, fmin(p3.x, p4.x)));
			float miny = fmin(p1.y, fmin(p2.y, fmin(p3.y, p4.y)));
			float maxx = fmax(p1.x, fmax(p2.x, fmax(p3.x, p4.x)));
			float maxy = fmax(p1.y, fmax(p2.y, fmax(p3.y, p4.y)));

			aabb.x = minx;
			aabb.y = miny;
			aabb.w = maxx - minx;
			aabb.h = maxy - miny;
		}
	}

	void PhysicsComponent::StepBack()
	{
		if (isStatic || histIdx == maxRewind)
			return;

		histIdx--;
		if (histIdx < 0)
			histIdx = PHYS_HISTORY_SIZE - 1;

		mat = physHistory[histIdx].mat;
		linearVelocity = physHistory[histIdx].vel;
		CalculateAABB();
		steppedBack = true;
	}

	void PhysicsComponent::StepForward(Physics* p)
	{
		if (isStatic)
			return;

		if (steppedBack) // if we stepped back last, do an extra step forward the first time
		{
			histIdx++;
			steppedBack = false;
		}

		if (histIdx == (maxForward + 1) % PHYS_HISTORY_SIZE)
		{
			Update(p, tickRate);
			return;
		}

		mat = physHistory[histIdx].mat;
		linearVelocity = physHistory[histIdx].vel;

		CalculateAABB();

		histIdx++;
		if (histIdx >= PHYS_HISTORY_SIZE)
			histIdx = 0;
	}

	void PhysicsComponent::Rewind()
	{
		rewind = true;
	}

	void PhysicsComponent::Forward()
	{
		rewind = false;
	}

	bool PhysicsComponent::ContainsPoint(float x, float y)
	{
		Panel* panel = nullptr;
		if (eid >= 0)
		{
			Entity* e = ScreenManager::Get()->GetEntity(eid);
			if (e->m_drawID >= 0)
			{
				auto d = ScreenManager::Get()->DrawManager()->GetComponent(e->m_drawID);
				if (d)
					panel = d->m_panel;
			}
		}

		if (m_type == PhysType::Box)
		{
			float xpos = aabb.x;
			float ypos = aabb.y;

			if (panel)
			{
				xpos += panel->rect.x;
				ypos += panel->rect.y;
			}

			Rect SS{xpos, ypos, aabb.w, aabb.h};
			return SS.Contains(x, y);
		}
		else if (m_type == PhysType::Sphere)
		{
			float xpos = pos().x;
			float ypos = pos().y;

			if (panel)
			{
				xpos += panel->rect.x;
				ypos += panel->rect.y;
			}

			Vec2f point(x, y);
			Vec2f center(xpos, ypos);
			float dist = (point - center).squareMag();
			return dist <= (radius * radius);

			// r->DrawCircle({xpos,ypos}, radius, colliding ? Color::Red() : Color::White());
			// r->DrawLine(xpos, ypos, xpos + (linearVelocity.x * 0.2f), ypos + (linearVelocity.y * 0.2f),
			// Color::Red());
		}
		return false;
	}

	void PhysicsComponent::Draw(Renderer* r)
	{
		Panel* panel = nullptr;
		if (eid >= 0)
		{
			Entity* e = ScreenManager::Get()->GetEntity(eid);
			if (e->m_drawID >= 0)
			{
				auto d = ScreenManager::Get()->DrawManager()->GetComponent(e->m_drawID);
				if (d)
					panel = d->m_panel;
			}
		}

		if (m_type == PhysType::Box)
		{
			float xpos = aabb.x + aabb.w / 2.0f;
			float ypos = aabb.y + aabb.h / 2.0f;

			if (panel)
			{
				xpos += panel->rect.x;
				ypos += panel->rect.y;
			}

			r->DrawEmptyQuad({xpos, ypos, aabb.w, aabb.h}, colliding ? Color::Red() : Color::White());
		}
		else if (m_type == PhysType::Sphere)
		{
			float xpos = pos().x;
			float ypos = pos().y;

			if (panel)
			{
				xpos += panel->rect.x;
				ypos += panel->rect.y;
			}

			r->DrawCircle({xpos, ypos}, radius, colliding ? Color::Red() : Color::White());
			r->DrawLine(xpos, ypos, xpos + (linearVelocity.x * 0.2f), ypos + (linearVelocity.y * 0.2f), Color::Red());
		}
	}

	Vec2f PhysicsComponent::pos()
	{
		return mat.pos();
	}

	void PhysicsComponent::Serialize(std::ofstream& ofs)
	{
		ofs << "PhysicsComponent" << '\n';

		int type = 0;
		switch (m_type)
		{
			case PhysType::Box:
				type = 0;
				break;
			case PhysType::Sphere:
				type = 1;
				break;
		}
		ofs << type << '\n';

		ofs << eid << '\n';
		ofs << alive << '\n';
		ofs << resolved << '\n';
		ofs << colliding << '\n';
		ofs << isStatic << '\n';
		ofs << isKinematic << '\n';
		ofs << rewind << '\n';

		ofs << mat << '\n';
		ofs << inertia << '\n';
		ofs << linearVelocity << '\n';
		ofs << angularVelocity << '\n';
		ofs << invmass << '\n';
		ofs << aabb << '\n';
		ofs << width << '\n';
		ofs << height << '\n';
		ofs << radians << '\n';
		ofs << radius << '\n';
	}

	void PhysicsComponent::Deserialize(std::ifstream& ifs)
	{
		std::string line;
		ifs >> line; //"PhysicsComponent";

		int type = 0;
		ifs >> type;
		switch (type)
		{
			case 0:
				m_type = PhysType::Box;
				break;
			case 1:
				m_type = PhysType::Sphere;
				break;
		}

		ifs >> eid;
		ifs >> alive;
		ifs >> resolved;
		ifs >> colliding;
		ifs >> isStatic;
		ifs >> isKinematic;
		ifs >> rewind;

		ifs >> mat;
		ifs >> inertia;
		ifs >> linearVelocity;
		ifs >> angularVelocity;
		ifs >> invmass;
		ifs >> aabb;
		ifs >> width;
		ifs >> height;
		ifs >> radians;
		ifs >> radius;
	}
} // namespace age
