#ifndef _GRONER_
#define _GRONER_ 

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <vector>

/*
 * https ://jackschaedler.github.io/handwriting-recognition/
 */

namespace gro
{

	enum class direction4
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	enum class direction16
	{
		N,
		NNE,
		NE,
		ENE,
		E,
		ESE,
		SE,
		SSE,
		S,
		SSW,
		SW,
		WSW,
		W,
		WNW,
		NW,
		NNW
	};

	typedef struct vertex
	{
		uint16_t x;
		uint16_t y;
	}
	vertex;

	// Vertex Simplification
	void smoothVertices(std::vector<vertex>& vertices);
	void thinVertices(std::vector<vertex>& vertices, const uint16_t box_radius);

	// Direction/Corner Detection
	std::vector<direction4> findDirections4(std::vector<vertex>& vertices);
	std::vector<direction4> findDirectionChanges(std::vector<direction4>& directions);
	std::vector<direction4> findDirectionChanges(std::vector<vertex>& vertices);

	std::vector<direction16> findDirections16(std::vector<vertex>& vertices);
	std::vector<size_t> findCorners(std::vector<direction16>& directions);
	std::vector<size_t> findCorners(std::vector<vertex>& vertices);

	// Helper Functions


	// Character Recognition
	char recognizeNumeric(std::vector<vertex>& vertices, std::vector<direction4>& directionChanges, std::vector<size_t>& corners);
	char recognizeNumeric(std::vector<vertex>& vertices);

}

#endif
