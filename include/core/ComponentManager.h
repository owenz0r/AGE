#pragma once

#include "globals.h"
#include <fstream>
#include <functional>

namespace age
{
	struct Index
	{
		int idx = -1;

		Index(int i) : idx(i) {};

		bool operator<(const int other)
		{
			return idx < other;
		}

		int operator++(int)
		{
			return idx++;
		}

		operator int()
		{
			return idx;
		}

		bool isValid()
		{
			return idx >= 0;
		}
	};

	inline std::ifstream& operator>>(std::ifstream& ifs, Index& i)
	{
		ifs >> i.idx;
		return ifs;
	};

	template <class T> class ComponentManager
	{
	  public:
		Index m_num = 0;
		T m_array[MAX_COMPONENTS];

		int CreateComponent()
		{
			if (m_num < MAX_COMPONENTS)
			{
				return m_num++;
			}
			else
			{
				return -1;
			}
		}

		Index CreateComponent(int eid)
		{
			if (m_num < MAX_COMPONENTS)
			{
				m_array[m_num].eid = eid;
				return m_num++;
			}
			else
			{
				return -1;
			}
		}

		void Process(std::function<void(T&)> func)
		{
			for (int i = 0; i < m_num; ++i)
			{
				if (m_array[i].alive)
					func(m_array[i]);
			}
		}

		void Serialize(std::ofstream& ofs)
		{
			ofs << m_num << '\n';
			for (int i = 0; i < m_num; ++i)
				m_array[i].Serialize(ofs);
		}

		void Deserialize(std::ifstream& ifs)
		{
			ifs >> m_num; // << '\n';
			for (int i = 0; i < m_num; ++i)
				m_array[i].Deserialize(ifs);
		}

		T* GetComponentArray()
		{
			return m_array;
		}

		T* GetComponent(int cid)
		{
			if (cid >= 0 && cid < m_num)
			{
				return &m_array[cid];
			}
			else
			{
				return nullptr;
			}
		}

		void Kill(int cid)
		{
			if (cid < m_num)
				m_array[cid].alive = false;
		}

		void Clear()
		{
			m_num = 0;
		}
	};
} // namespace age
