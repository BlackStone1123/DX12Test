#pragma once
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		: vertices(std::move(verts_in))
		, indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};