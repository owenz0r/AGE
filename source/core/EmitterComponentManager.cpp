#include "core/EmitterComponentManager.h"

namespace age
{
	void EmitterComponentManager::Update(const double dt)
	{
		m_internal.Process([dt](EmitterComponent& c) { c.Update(dt); });
	}
} // namespace age
