#pragma once

#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <CoreFoundation/CoreFoundation.h>
#include <limits.h>
#elif defined(__linux__)
#include <unistd.h>
#else
#error "Unsupported platform for getExecutablePath()"
#endif

namespace age
{
	constexpr float DEG_TO_RAD = 3.14159265359 / 180.0;

	struct Point2Di
	{
		int x;
		int y;
	};

	struct Point2Df
	{
		float x;
		float y;

		Point2Df() : x(0.0f), y(0.0f) {};
		Point2Df(float a, float b) : x(a), y(b) {};

		static Point2Df Zero()
		{
			return {0.0f, 0.0f};
		};

		bool isZero()
		{
			return x == 0.0f && y == 0.0f;
		}

		float dot(const Point2Df& other)
		{
			return (this->x * other.x) + (this->y * other.y);
		}

		float mag()
		{
			return sqrt((x * x) + (y * y));
		}

		float squareMag()
		{
			return (x * x) + (y * y);
		}

		Point2Df normalized()
		{
			float m = mag();
			return m > 0.0f ? Point2Df(x / m, y / m) : Point2Df();
		}

		Point2Df reflected(Point2Df& normal)
		{
			return Point2Df(*this - normal * 2 * (this->dot(normal)));
		}

		Point2Df perp()
		{
			return Point2Df(-y, x);
		}

		bool almostEqual(const Point2Df& other)
		{
			if (abs(x - other.x) < 0.0005f && abs(y - other.y) < 0.0005f)
			{
				return true;
			}
			return false;
		}

		Point2Df operator*(double d)
		{
			return Point2Df(this->x * d, this->y * d);
		}

		Point2Df operator-(const Point2Df& other)
		{
			return Point2Df(this->x - other.x, this->y - other.y);
		}

		Point2Df operator-(const Point2Df& other) const
		{
			return Point2Df(this->x - other.x, this->y - other.y);
		}

		Point2Df operator+(const Point2Df& other)
		{
			return Point2Df(this->x + other.x, this->y + other.y);
		}

		Point2Df& operator+=(Point2Df& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		Point2Df& operator+=(Point2Df&& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		float operator%(Point2Df& rhs)
		{
			return (this->x * rhs.y) - (this->y * rhs.x);
		}
	};

	typedef Point2Df Vec2f;

	inline std::ofstream& operator<<(std::ofstream& ofs, const Point2Df& p)
	{
		ofs << p.x << ' ';
		ofs << p.y;
		return ofs;
	};

	inline std::ifstream& operator>>(std::ifstream& ifs, Point2Df& p)
	{
		ifs >> p.x;
		ifs >> p.y;
		return ifs;
	};

	struct Mat3f
	{
		float m[9];

		Mat3f()
		{
			identity();
		};

		void identity()
		{
			m[0] = 1.0f; // x-axis { m[0], m[1], m[2] }
			m[1] = 0.0f;
			m[2] = 0.0f;

			m[3] = 0.0f; // y-axis { m[3], m[4], m[5] }
			m[4] = 1.0f;
			m[5] = 0.0f;

			m[6] = 0.0f; // tranlate { m[6], m[7], m[8] }
			m[7] = 0.0f;
			m[8] = 1.0f;
		}

		static Mat3f CreateRotation(const float radians)
		{
			Mat3f result;

			result.m[0] = cos(radians);
			result.m[1] = sin(radians);
			result.m[2] = 0.0f;

			result.m[3] = -sin(radians);
			result.m[4] = cos(radians);
			result.m[5] = 0.0f;

			result.m[6] = 0.0f;
			result.m[7] = 0.0f;
			result.m[8] = 1.0f;

			return result;
		}

		static Mat3f CreateTranslate(const float x, const float y)
		{
			Mat3f result;

			result.m[0] = 1.0f;
			result.m[1] = 0.0f;
			result.m[2] = 0.0f;

			result.m[3] = 0.0f;
			result.m[4] = 1.0f;
			result.m[5] = 0.0f;

			result.m[6] = x;
			result.m[7] = y;
			result.m[8] = 1.0f;

			return result;
		}

		void operator<<(std::ofstream& ofs)
		{
		}

		Vec2f operator*(const Vec2f& vec)
		{
			return Vec2f(vec.x * m[0] + vec.y * m[3] + m[6], vec.x * m[1] + vec.y * m[4] + m[7]);
		}

		Vec2f operator*(const Vec2f& vec) const
		{
			return Vec2f(vec.x * m[0] + vec.y * m[3] + m[6], vec.x * m[1] + vec.y * m[4] + m[7]);
		}

		Mat3f operator*(const Mat3f& other)
		{
			Mat3f result;

			result.m[0] = m[0] * other.m[0] + m[1] * other.m[3] + m[2] * other.m[6];
			result.m[1] = m[0] * other.m[1] + m[1] * other.m[4] + m[2] * other.m[7];
			result.m[2] = m[0] * other.m[2] + m[1] * other.m[5] + m[2] * other.m[8];

			result.m[3] = m[3] * other.m[0] + m[4] * other.m[3] + m[5] * other.m[6];
			result.m[4] = m[3] * other.m[1] + m[4] * other.m[4] + m[5] * other.m[7];
			result.m[5] = m[3] * other.m[2] + m[4] * other.m[5] + m[5] * other.m[8];

			result.m[6] = m[6] * other.m[0] + m[7] * other.m[3] + m[8] * other.m[6];
			result.m[7] = m[6] * other.m[1] + m[7] * other.m[4] + m[8] * other.m[7];
			result.m[8] = m[6] * other.m[2] + m[7] * other.m[5] + m[8] * other.m[8];

			return result;
		}

		void translate(const float x, const float y)
		{
			m[6] += x;
			m[7] += y;
		}

		void translate(const Vec2f& t)
		{
			m[6] += t.x;
			m[7] += t.y;
		}

		Vec2f pos()
		{
			return {m[6], m[7]};
		}

		//	Mat3f inverse()
		//	{
		//		Mat3f result;
		//		float det = m[0] * m[4] - m[3] * m[1];
		//		if (det <= 0)
		//			return Mat3f();
		//		result.m[0] = m[4] / det;
		//		result.m[1] = -m[1] / det;
		//		result.m[2] = 0.0f;
		//
		//		result.m[3] = -m[3] / det;
		//		result.m[4] = m[0] / det;
		//		result.m[5] = 0.0f;
		//
		//		result.m[6] = -m[6];
		//		result.m[7] = -m[7];
		//		result.m[8] = 1.0f;
		//
		//		return result;
		//	}

		Mat3f inverse()
		{
			float det = m[0] * (m[4] * m[8] - m[7] * m[5]) - m[1] * (m[3] * m[8] - m[5] * m[6]) +
						m[2] * (m[3] * m[7] - m[4] * m[6]);

			float invdet = 1.0f / det;

			Mat3f result;										// inverse of matrix m
			result.m[0] = (m[4] * m[8] - m[7] * m[5]) * invdet; // 0
			result.m[1] = (m[2] * m[7] - m[1] * m[8]) * invdet; // 1
			result.m[2] = (m[1] * m[5] - m[2] * m[4]) * invdet; // 2
			result.m[3] = (m[5] * m[6] - m[3] * m[8]) * invdet; // 3
			result.m[4] = (m[0] * m[8] - m[2] * m[6]) * invdet; // 4
			result.m[5] = (m[3] * m[2] - m[0] * m[5]) * invdet; // 5
			result.m[6] = (m[3] * m[7] - m[6] * m[4]) * invdet; // 6
			result.m[7] = (m[6] * m[1] - m[0] * m[7]) * invdet; // 7
			result.m[8] = (m[0] * m[4] - m[3] * m[1]) * invdet; // 8

			return result;
		}

		void rotate(float radians)
		{
			Mat3f tmpMat = *this;
			tmpMat.m[6] = 0.0f;
			tmpMat.m[7] = 0.0f;
			tmpMat = tmpMat * CreateRotation(radians);
			tmpMat.m[6] = m[6];
			tmpMat.m[7] = m[7];
			*this = tmpMat;
			//*this = *this * CreateRotation(radians);
		}

		float radianRotation()
		{
			Vec2f tmp{1.0f, 0.0f};
			Mat3f tmpMat = *this;
			tmpMat.m[6] = 0.0f;
			tmpMat.m[7] = 0.0f;
			tmp = tmpMat * tmp;
			return atan2(tmp.y, tmp.x);
		}
	};

	inline std::ofstream& operator<<(std::ofstream& ofs, const Mat3f& mat)
	{
		ofs << mat.m[0] << ' ';
		ofs << mat.m[1] << ' ';
		ofs << mat.m[2] << ' ';

		ofs << mat.m[3] << ' ';
		ofs << mat.m[4] << ' ';
		ofs << mat.m[5] << ' ';

		ofs << mat.m[6] << ' ';
		ofs << mat.m[7] << ' ';
		ofs << mat.m[8];

		return ofs;
	};

	inline std::ifstream& operator>>(std::ifstream& ifs, Mat3f& mat)
	{
		ifs >> mat.m[0];
		ifs >> mat.m[1];
		ifs >> mat.m[2];

		ifs >> mat.m[3];
		ifs >> mat.m[4];
		ifs >> mat.m[5];

		ifs >> mat.m[6];
		ifs >> mat.m[7];
		ifs >> mat.m[8];

		return ifs;
	};

	static float toRadians(const float degrees)
	{
		return DEG_TO_RAD * degrees;
	}

	struct Color
	{
		int r;
		int g;
		int b;
		int a;

		static constexpr Color Red()
		{
			return {255, 0, 0, 255};
		};
		static constexpr Color Green()
		{
			return {0, 255, 0, 255};
		};
		static constexpr Color Blue()
		{
			return {0, 0, 255, 255};
		};
		static constexpr Color Yellow()
		{
			return {255, 255, 0, 255};
		};
		static constexpr Color Cyan()
		{
			return {0, 255, 255, 255};
		};
		static constexpr Color Purple()
		{
			return {255, 0, 255, 255};
		};
		static constexpr Color White()
		{
			return {255, 255, 255, 255};
		};
		static constexpr Color Black()
		{
			return {0, 0, 0, 255};
		};
	};

	inline std::ofstream& operator<<(std::ofstream& ofs, const Color& c)
	{
		ofs << c.r << ' ';
		ofs << c.g << ' ';
		ofs << c.b << ' ';
		ofs << c.a;
		return ofs;
	};

	inline std::ifstream& operator>>(std::ifstream& ifs, Color& c)
	{
		ifs >> c.r;
		ifs >> c.g;
		ifs >> c.b;
		ifs >> c.a;
		return ifs;
	};

	struct Rect
	{
		Rect() = default;
		Rect(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {};
		float x, y;
		float w, h;

		bool Contains(float u, float v)
		{
			return u >= x && u <= (x + w) && v >= y && v <= (y + h);
		}
	};

	inline std::ofstream& operator<<(std::ofstream& ofs, const Rect& r)
	{
		ofs << r.x << ' ';
		ofs << r.y << ' ';
		ofs << r.w << ' ';
		ofs << r.h;
		return ofs;
	};

	inline std::ifstream& operator>>(std::ifstream& ifs, Rect& r)
	{
		ifs >> r.x;
		ifs >> r.y;
		ifs >> r.w;
		ifs >> r.h;
		return ifs;
	};

	struct SphereParams
	{
		Vec2f pos = {0.0f, 0.0f};
		float rot = 0.0f;
		float radius = 1.0f;
		float angVel = 0.0f;
		Vec2f linVel = {0.0f, 0.0f};
		float mass = 1.0f;
		std::string texture = "ballBlue.png";
	};

	struct BoxParams
	{
		Vec2f pos = {0.0f, 0.0f};
		float rot = 0.0f;
		float width = 1.0f;
		float height = 1.0f;
		float angVel = 0.0f;
		Vec2f linVel = {0.0f, 0.0f};
		float mass = 1.0f;
		std::string texture = "element_yellow_square.png";
	};

	struct PhysInfo
	{
		Mat3f mat;
		Vec2f vel;
	};

	inline std::filesystem::path getExecutablePath()
	{
		std::filesystem::path exePath;

#if defined(_WIN32)
		wchar_t buffer[MAX_PATH];
		DWORD len = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		if (len > 0)
			exePath = std::filesystem::path(buffer);
#elif defined(__APPLE__)
		uint32_t size = 0;
		_NSGetExecutablePath(nullptr, &size);
		std::string buffer(size, '\0');
		if (_NSGetExecutablePath(buffer.data(), &size) == 0)
			exePath = std::filesystem::path(buffer).lexically_normal();
#elif defined(__linux__)
		char buffer[4096];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1)
		{
			buffer[len] = '\0';
			exePath = std::filesystem::path(buffer);
		}
#endif
		// Resolve symlinks, ., .., etc.
		if (!exePath.empty() && std::filesystem::exists(exePath))
			exePath = std::filesystem::canonical(exePath);

		return exePath;
	}

#if defined(__APPLE__)
	inline std::string getResourcesPath()
	{
		CFURLRef url =
		CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
		
		char path[PATH_MAX];
		CFURLGetFileSystemRepresentation(
										 url, true, reinterpret_cast<UInt8*>(path), PATH_MAX);
		
		CFRelease(url);
		return path;
	}
#else
	inline std::filesystem::path getResourcesPath()
	{
		return getExecutablePath().parent_path() / "Resources";
	}
#endif
} // namespace age
