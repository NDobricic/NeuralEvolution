#pragma once
#include "MapObject.h"

namespace evol
{
	class Food : public MapObject
	{
	private:
		Coord<uint16_t> pos;

	public:
		Food(uint16_t x, uint16_t y)
			: pos(x, y) { }

		virtual Color GetColor() const override
		{
			return Color(255, 255, 0);
		}

		virtual bool IsObstacle() const override
		{
			return false;
		}

		virtual Coord<uint16_t> GetPosition() const override
		{
			return pos;
		}
	};
}
