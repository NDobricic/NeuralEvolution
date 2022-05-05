#pragma once

#include "Color.h"
#include "Coord.h"

namespace evol
{
	class MapObject
	{
	public:
		virtual Coord<uint16_t> GetPosition() const = 0;
		virtual Color GetColor() const = 0;
		virtual bool IsObstacle() const = 0;
	};
}