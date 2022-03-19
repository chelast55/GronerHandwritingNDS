#include <iostream>

#include "../../../source/groner.hpp"

int main()
{
	// shape of '2'
	std::vector<gro::vertex> vertices
	({ 
		{70,51}, 
		{74,49}, 
		{83,46},
		{95,42},
		{106,39},
		{119,37}, 
		{132,37}, 
		{142,38}, 
		{152,41}, 
		{162,45}, 
		{170,50}, 
		{171,58}, 
		{172,65}, 
		{163,77}, 
		{153,91}, 
		{136,105}, 
		{124,115}, 
		{116,120}, 
		{107,125}, 
		{105,126}, 
		{104,126}, 
		{101,126}, 
		{95,127}, 
		{91,129},
		{90,129},
		{90,129},
		{90,129},
		{91,130},
		{91,130},
		{98,130},
		{109,132},
		{128,135},
		{156,142},
		{183,147},
		{201,148},
		{214,150},
		{218,150},
		{218,150},
		{218,150},
		{218,150}
	});

	gro::smoothVertices(vertices);

	const uint16_t box_radius = 4;

	gro::thinVertices(vertices, box_radius);

	std::vector<gro::direction4> directions = gro::findDirectionChanges(vertices);

	//std::vector<gro::direction16> sky_directions = gro::findDirection16(vertices);

	std::vector<gro::direction16> sky_directions
	({
		gro::direction16::NE,
		gro::direction16::ENE,
		gro::direction16::E,
		gro::direction16::SE,
		gro::direction16::SSW,
		gro::direction16::SW,
		gro::direction16::SW,
		gro::direction16::SW,
		gro::direction16::SW,
		gro::direction16::WSW,
		gro::direction16::WSW,
		gro::direction16::ESE,
		gro::direction16::ESE,
		gro::direction16::E,
		gro::direction16::E,
		gro::direction16::E
	});

	std::vector<size_t> corners = gro::findCorners(sky_directions);

	return 1;
}