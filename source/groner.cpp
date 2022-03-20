#include "groner.hpp"

constexpr uint16_t ZERO_SIX_DIFFERENCE_SIGNIFICANCE_FACTOR = 4;

void gro::smoothVertices(std::vector<vertex>& vertices)
{
	if (vertices.size() > 1)
	{
		uint16_t x_0_div, y_0_div;
		uint16_t x_1_div, y_1_div;
		for (size_t i = 1; i < vertices.size(); i++)
		{
			// get v_0 * 0.5
			x_0_div = vertices[i - 1].x >> 1;
			y_0_div = vertices[i - 1].y >> 1;

			// get  (v_0 * 0.5) + (v_0 * 0.25) = v_0 * 0.75
			x_0_div += vertices[i - 1].x >> 2;
			y_0_div += vertices[i - 1].y >> 2;

			// get v_1 * 0.25
			x_1_div = vertices[i].x >> 2;
			y_1_div = vertices[i].y >> 2;

			// calculate (0.75 * v_0) + (0.25 * v_1)
			vertices[i].x = x_0_div + x_1_div;
			vertices[i].y = y_0_div + y_1_div;
		}
	}
}

void gro::thinVertices(std::vector<vertex>& vertices, const uint16_t box_radius)
{
	if (vertices.size() > 1)
	{
		std::vector<size_t> redundant_vertices({}); // by index in vertices
		const size_t vertices_size_decrement = vertices.size() - 1;
		size_t temp_i;
		for (size_t i = 0; i < vertices_size_decrement; i++)
		{
			temp_i = i;
			// mark all upcoming vertices within box_radius of vertex_i as redundant
			for (size_t j = i + 1; j < vertices_size_decrement; j++)
			{
				if (std::abs(vertices[i].x - vertices[j].x) <= box_radius
					|| std::abs(vertices[i].y - vertices[j].y) <= box_radius) 
				{ 
					redundant_vertices.push_back(j);
					temp_i++; // skip this redundant vertex for box checking later
				}
				else
				{
					break; // all redundant vertices for vertex_i were found
				}
			}
			i = temp_i; // jump to next non-redundant vertex
		}

		// delete all redundant vertices form vertices starting from the back
		for (size_t i = redundant_vertices.size() - 1; i != (size_t)-1; i--) // provoke overflow
		{
			vertices.erase(vertices.begin() + redundant_vertices[i]);
		}
	}
}

std::vector<gro::direction4> gro::findDirections4(std::vector<vertex>& vertices)
{
	std::vector<direction4> directions({});
	if (vertices.size() > 1)
	{
		int delta_x, delta_y;
		for (size_t i = 1; i < vertices.size(); i++)
		{
			delta_x = vertices[i].x - vertices[i - 1].x;
			delta_y = vertices[i].y - vertices[i - 1].y; 
			if (std::abs(delta_x) >= std::abs(delta_y)) // horizontal
			{
				if (delta_x >= 0)
					directions.push_back(direction4::RIGHT);
				else
					directions.push_back(direction4::LEFT);
			}
			else // vertical
			{
				if (delta_y < 0)
					directions.push_back(direction4::UP);
				else
					directions.push_back(direction4::DOWN); 
			}
		}
	}
	return directions;
}

std::vector<gro::direction4> gro::findDirectionChanges(std::vector<direction4>& directions)
{
	std::vector<direction4> direction_changes({});
	if (directions.size() > 1)
	{
		direction4 previous_direction = directions[0];
		direction_changes.push_back(previous_direction);
		for (size_t i = 1; i < directions.size(); i++)
		{
			if (directions[i] != previous_direction)
			{
				previous_direction = directions[i];
				direction_changes.push_back(previous_direction);
			}
		}
	}
	return direction_changes;
}

std::vector<gro::direction4> gro::findDirectionChanges(std::vector<vertex>& vertices)
{
	std::vector<direction4> directions = findDirections4(vertices);
	return findDirectionChanges(directions);
}

std::vector<gro::direction16> gro::findDirections16(std::vector<vertex>& vertices)
{
	std::vector<direction16> directions({});
	if (vertices.size() > 1)
	{
		int delta_x, delta_y;
		for (size_t i = 1; i < vertices.size(); i++)
		{
			delta_x = vertices[i].x - vertices[i - 1].x;
			delta_y = vertices[i].y - vertices[i - 1].y;
			if (std::abs(delta_x) >= std::abs(delta_y)) // horizontal
			{
				if (delta_x >= 0) // east
				{
					if (delta_y < 0) // north-east
					{
						if (std::abs(delta_y) < (std::abs(delta_x) >> 2)) // y < 0.25x
							directions.push_back(direction16::E);
						else if (std::abs(delta_y) < ((std::abs(delta_x) >> 1) + (std::abs(delta_x) >> 2))) // 0.25x <= y < 0.75x
							directions.push_back(direction16::ENE);
						else // 0.75x < y
							directions.push_back(direction16::NE);
					}
					else // south-east
					{
						if (std::abs(delta_y) < (std::abs(delta_x) >> 2)) // y < 0.25x
							directions.push_back(direction16::E);
						else if (std::abs(delta_y) < ((std::abs(delta_x) >> 1) + (std::abs(delta_x) >> 2))) // 0.25x <= y < 0.75x
							directions.push_back(direction16::ESE);
						else // 0.75x < y
							directions.push_back(direction16::SE);
					}
				}
				else // west
				{
					if (delta_y < 0) // north-west
					{
						if (std::abs(delta_y) < (std::abs(delta_x) >> 2)) // y < 0.25x
							directions.push_back(direction16::W);
						else if (std::abs(delta_y) < ((std::abs(delta_x) >> 1) + (std::abs(delta_x) >> 2))) // 0.25x <= y < 0.75x
							directions.push_back(direction16::WNW);
						else // 0.75x < y
							directions.push_back(direction16::NW);
					}
					else // south-west
					{
						if (std::abs(delta_y) < (std::abs(delta_x) >> 2)) // y < 0.25x
							directions.push_back(direction16::W);
						else if (std::abs(delta_y) < ((std::abs(delta_x) >> 1) + (std::abs(delta_x) >> 2))) // 0.25x <= y < 0.75x
							directions.push_back(direction16::WSW);
						else // 0.75x < y
							directions.push_back(direction16::SW);
					}
				}
			}
			else // vertical
			{
				if (delta_y < 0)
				{
					if (delta_x >= 0) // north-east
					{
						if (std::abs(delta_x) < (std::abs(delta_y) >> 2)) // x < 0.25y
							directions.push_back(direction16::N);
						else if (std::abs(delta_x) < ((std::abs(delta_y) >> 1) + (std::abs(delta_y) >> 2))) // 0.25y <= x < 0.75y
							directions.push_back(direction16::NNE);
						else // 0.75y < x
							directions.push_back(direction16::NE);
					}
					else // north-west
					{
						if (std::abs(delta_x) < (std::abs(delta_y) >> 2)) // x < 0.25y
							directions.push_back(direction16::N);
						else if (std::abs(delta_x) < ((std::abs(delta_y) >> 1) + (std::abs(delta_y) >> 2))) // 0.25y <= x < 0.75y
							directions.push_back(direction16::NNW);
						else // 0.75y < x
							directions.push_back(direction16::NW);
					}
				}
				else
				{
					if (delta_x >= 0) // south-east
					{
						if (std::abs(delta_x) < (std::abs(delta_y) >> 2)) // x < 0.25y
							directions.push_back(direction16::S);
						else if (std::abs(delta_x) < ((std::abs(delta_y) >> 1) + (std::abs(delta_y) >> 2))) // 0.25y <= x < 0.75y
							directions.push_back(direction16::SSE);
						else // 0.75y < x
							directions.push_back(direction16::SE);
					}
					else // south-west
					{
						if (std::abs(delta_x) < (std::abs(delta_y) >> 2)) // x < 0.25y
							directions.push_back(direction16::S);
						else if (std::abs(delta_x) < ((std::abs(delta_y) >> 1) + (std::abs(delta_y) >> 2))) // 0.25y <= x < 0.75y
							directions.push_back(direction16::SSW);
						else // 0.75y < x
							directions.push_back(direction16::SW);
					}
				}
			}
		}
	}
	return directions;
}

std::vector<size_t> gro::findCorners(std::vector<direction16>& directions)
{
	std::vector<size_t> corners({});
	if (directions.size() > 3)
	{
		for (size_t i = 3; i < directions.size(); i++)
		{
			int8_t distance;
			if (directions[i - 3] == directions[i - 2]
				&& directions[i - 1] == directions[i])
			{
				distance = std::abs((int8_t)directions[i - 1] - (int8_t)directions[i - 2]);
				if (distance >= 4 && distance < 12) // 4+ steps apart equals 90°+
					corners.push_back(i - 1);
			}
		}
		
	}
	return corners;
}

std::vector<size_t> gro::findCorners(std::vector<vertex>& vertices)
{
	std::vector<direction16> directions = findDirections16(vertices);
	return findCorners(directions);
}

char gro::recognizeNumeric(std::vector<vertex>& vertices, std::vector<direction4>& directionChanges, std::vector<size_t>& corners)
{
	if (directionChanges.size() == 1)
	{
		return '1'; // straight line 1
	}
	else if (directionChanges.size() <= 2)
	{
		if ((directionChanges[0] == direction4::DOWN && directionChanges[1] != direction4::UP)
			|| (directionChanges[0] == direction4::UP && directionChanges[1] == direction4::DOWN))
			return '1'; // "crooked" line or narrow 1
		else if (directionChanges[0] == direction4::RIGHT && directionChanges[1] == direction4::DOWN)
			return '7'; // right-angled 7
	}
	else if (directionChanges.size() <= 3)
	{
		if (directionChanges[0] == direction4::RIGHT)
		{
			if (directionChanges[1] == direction4::DOWN)
			{
				if (directionChanges[2] == direction4::RIGHT)
					return '2'; // somewhat straight 2
				else if (directionChanges[2] == direction4::LEFT)
					return '7'; // sharp-angled 7
			}
			else if (directionChanges[1] == direction4::UP && directionChanges[2] == direction4::DOWN)
				return '1'; // wide 1 with roof
		}
		else if (directionChanges[0] == direction4::LEFT
			&& directionChanges[1] == direction4::DOWN 
			&& directionChanges[2] == direction4::LEFT) 
		{
			return '5'; // minimalistic 5
		}
		else if (directionChanges[0] == direction4::UP
			&& directionChanges[1] == direction4::LEFT 
			&& directionChanges[2] == direction4::DOWN) 
		{
			return '7'; // right-angled 7 with serif
		}
	}
	else if (directionChanges.size() <= 4)
	{
		// TODO: consider clockwise 0s
		if (directionChanges[0] == direction4::UP 
			&& directionChanges[1] == direction4::RIGHT
			&& directionChanges[2] == direction4::DOWN)
		{
			if (directionChanges[3] == direction4::RIGHT)
				return '2'; // somewhat straight 2 with curve at beginning
			else if (directionChanges[3] == direction4::LEFT)
				return '7'; // sharp-angled 7 with serif
		}
		else if (directionChanges[0] == direction4::LEFT)
		{
			if (directionChanges[1] == direction4::UP
				&& directionChanges[2] == direction4::RIGHT
				&& directionChanges[3] == direction4::DOWN)
			{
				return '9'; // simple straight 9
			}
			else if (directionChanges[1] == direction4::RIGHT
				&& directionChanges[2] == direction4::DOWN
				&& directionChanges[3] == direction4::LEFT)
			{
				return '5'; // 5 as upside down "somewhat straight 2 with curve at beginning"
			}
			else if (directionChanges[1] == direction4::DOWN
				&& directionChanges[2] == direction4::LEFT
				&& directionChanges[3] == direction4::UP)
			{
				return '0'; // 0 starting from upper right
			}
		}
		else if (directionChanges[0] == direction4::DOWN
			&& directionChanges[1] == direction4::RIGHT
			&& directionChanges[2] == direction4::UP) 
		{
			if (directionChanges[3] == direction4::DOWN)
				return '4'; // straight 4
			else if (directionChanges[3] == direction4::LEFT)
			{
				uint16_t delta_x = std::abs(vertices[0].x - vertices[vertices.size() - 1].x);
				uint16_t delta_y = std::abs(vertices[0].y - vertices[vertices.size() - 1].y);
				if (delta_y > ZERO_SIX_DIFFERENCE_SIGNIFICANCE_FACTOR * delta_x)
					return '6'; // simple straight 6
				else
					return '0'; // 0 starting from upper left
			}
		}
		else if (directionChanges[0] == direction4::RIGHT)
		{
			if (directionChanges[1] == direction4::DOWN
				&& directionChanges[2] == direction4::LEFT
				&& directionChanges[3] == direction4::RIGHT)
			{
				return '2'; // angled 2 with flat top
			}
			else if (directionChanges[1] == direction4::LEFT
				&& directionChanges[2] == direction4::RIGHT
				&& directionChanges[3] == direction4::LEFT)
			{
				return '3'; // pointy 3
			}
		}
	}
	else if (directionChanges.size() <= 5)
	{
		// curved 6
		// clockwise curved 9
		// counterclockwise curved 9
		// stepped 5
		// pointy upper, round lower 3
		// round upper, pointy lower 3
		// angled 2 with round top
	}
	else if (directionChanges.size() <= 6)
	{ 
		// curved stepped 5
		// curved 3
		// clockhise hourglass 8
		// counterclockwise hourglass 8
	}
	else if (directionChanges.size() <= 8)
	{
		// clockwhise round 8
		// counterclockwise round 8
	}
	else
	{
		// tangled mess that probably is an 8
	}

	return '#'; // "Error" case, no number found (in this step) 
}

char gro::recognizeNumeric(std::vector<vertex>& vertices)
{
	std::vector<direction4> directionChanges = findDirectionChanges(vertices);
	std::vector<size_t> corners = findCorners(vertices);
	return recognizeNumeric(vertices, directionChanges, corners);
}
