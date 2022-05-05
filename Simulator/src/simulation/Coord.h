#pragma once

template<typename T>
struct Coord
{
	T x, y;

	Coord() : x(0), y(0) { }
	Coord(T x, T y) : x(x), y(y) { }
};