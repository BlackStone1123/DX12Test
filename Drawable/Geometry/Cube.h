#pragma once
#include "Vertex.h"
#include <initializer_list>

namespace dx = DirectX;

class Cube
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices( 8 );
		vertices[0].pos = { -side,-side,-side };
		vertices[1].pos = { side,-side,-side };
		vertices[2].pos = { -side,side,-side };
		vertices[3].pos = { side,side,-side };
		vertices[4].pos = { -side,-side,side };
		vertices[5].pos = { side,-side,side };
		vertices[6].pos = { -side,side,side };
		vertices[7].pos = { side,side,side };

		return{
			std::move( vertices ),{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}
	template<class V>
	static IndexedTriangleList<V> MakeSkinned()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices( 14 );

		vertices[0].pos = { -side,-side,-side };
		vertices[0].tex = { 2.0f / 3.0f,0.0f / 4.0f };
		vertices[1].pos = { side,-side,-side };
		vertices[1].tex = { 1.0f / 3.0f,0.0f / 4.0f };
		vertices[2].pos = { -side,side,-side };
		vertices[2].tex = { 2.0f / 3.0f,1.0f / 4.0f };
		vertices[3].pos = { side,side,-side };
		vertices[3].tex = { 1.0f / 3.0f,1.0f / 4.0f };
		vertices[4].pos = { -side,-side,side };
		vertices[4].tex = { 2.0f / 3.0f,3.0f / 4.0f };
		vertices[5].pos = { side,-side,side };
		vertices[5].tex = { 1.0f / 3.0f,3.0f / 4.0f };
		vertices[6].pos = { -side,side,side };
		vertices[6].tex = { 2.0f / 3.0f,2.0f / 4.0f };
		vertices[7].pos = { side,side,side };
		vertices[7].tex = { 1.0f / 3.0f,2.0f / 4.0f };
		vertices[8].pos = { -side,-side,-side };
		vertices[8].tex = { 2.0f / 3.0f,4.0f / 4.0f };
		vertices[9].pos = { side,-side,-side };
		vertices[9].tex = { 1.0f / 3.0f,4.0f / 4.0f };
		vertices[10].pos = { -side,-side,-side };
		vertices[10].tex = { 3.0f / 3.0f,1.0f / 4.0f };
		vertices[11].pos = { -side,-side,side };
		vertices[11].tex = { 3.0f / 3.0f,2.0f / 4.0f };
		vertices[12].pos = { side,-side,-side };
		vertices[12].tex = { 0.0f / 3.0f,1.0f / 4.0f };
		vertices[13].pos = { side,-side,side };
		vertices[13].tex = { 0.0f / 3.0f,2.0f / 4.0f };

		return{
			std::move( vertices ),{
				0,2,1,   2,3,1,
				4,8,5,   5,8,9,
				2,6,3,   3,6,7,
				4,5,7,   4,7,6,
				2,10,11, 2,11,6,
				12,3,7,  12,7,13
			}
		};
	}
	template<class V>
	static IndexedTriangleList<V> MakeIndependent()
	{
		constexpr float side = 1.0f;

		std::vector<V> vertices( 24 );
		vertices[0].pos = { -side,-side,-side };// 0 near side
		vertices[0].tex = { 0.0f, 1.0f };
		vertices[1].pos = { side,-side,-side };// 1
		vertices[1].tex = { 1.0f, 1.0f };
		vertices[2].pos = { -side,side,-side };// 2
		vertices[2].tex = { 0.0f, 0.0f };
		vertices[3].pos = { side,side,-side };// 3
		vertices[3].tex = { 1.0f, 0.0f };
		vertices[4].pos = { -side,-side,side };// 4 far side
		vertices[4].tex = { 1.0f, 1.0f };
		vertices[5].pos = { side,-side,side };// 5
		vertices[5].tex = { 0.0f, 1.0f };
		vertices[6].pos = { -side,side,side };// 6
		vertices[6].tex = { 1.0f, 0.0f };
		vertices[7].pos = { side,side,side };// 7
		vertices[7].tex = { 0.0f, 0.0f };
		vertices[8].pos = { -side,-side,-side };// 8 left side
		vertices[8].tex = { 1.0f, 1.0f };
		vertices[9].pos = { -side,side,-side };// 9
		vertices[9].tex = { 1.0f, 0.0f };
		vertices[10].pos = { -side,-side,side };// 10
		vertices[10].tex = { 0.0f, 1.0f };
		vertices[11].pos = { -side,side,side };// 11
		vertices[11].tex = { 0.0f, 0.0f };
		vertices[12].pos = { side,-side,-side };// 12 right side
		vertices[12].tex = { 1.0f, 1.0f };
		vertices[13].pos = { side,side,-side };// 13
		vertices[13].tex = { 1.0f, 0.0f };
		vertices[14].pos = { side,-side,side };// 14
		vertices[14].tex = { 0.0f, 1.0f };
		vertices[15].pos = { side,side,side };// 15
		vertices[15].tex = { 0.0f, 0.0f };
		vertices[16].pos = { -side,-side,-side };// 16 bottom side
		vertices[16].tex = { 0.0f, 0.0f };
		vertices[17].pos = { side,-side,-side };// 17
		vertices[17].tex = { 1.0f, 0.0f };
		vertices[18].pos = { -side,-side,side };// 18
		vertices[18].tex = { 0.0f, 1.0f };
		vertices[19].pos = { side,-side,side };// 19
		vertices[19].tex = { 1.0f, 1.0f };
		vertices[20].pos = { -side,side,-side };// 20 top side
		vertices[20].tex = { 0.0f, 1.0f };
		vertices[21].pos = { side,side,-side };// 21
		vertices[21].tex = { 1.0f, 1.0f };
		vertices[22].pos = { -side,side,side };// 22
		vertices[22].tex = { 0.0f, 0.0f };
		vertices[23].pos = { side,side,side };// 23
		vertices[23].tex = { 1.0f, 0.0f };

		return{
			std::move( vertices ),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}
};