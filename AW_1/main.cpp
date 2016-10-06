#include <iostream>
#include <omp.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

struct Point
{
	int x, y, type;
	Point(int _x, int _y, int _t): x(_x), y(_y), type(_t)
	{}

	double distance(Point& other) const
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2));
	}

	void point_type() const
	{
		std::cout << type;
	}
	std::string to_string() const
	{
		std::stringstream ss;
		ss << "x: " << x << ", y: " << y << ", type: " << type;
		return ss.str();
	}
};

void initialize_learning_set(std::vector<Point> &learning_vector, unsigned int learning_set_count)
{
	std::string buf;
	for (unsigned int i = 0; i < learning_set_count; ++i)
	{
		int x, y, t;
		std::cin >> x >> y >> t;
		learning_vector.push_back(Point(x, y, t));
	}

}

void initialize_test_set(std::vector<Point> &test_vector, unsigned int test_set_count)
{
	std::string buf;
	for (unsigned int i = 0; i < test_set_count; ++i)
	{
		int x, y;
		std::cin >> x >> y;
		test_vector.push_back(Point(x, y, 0));
	}
}

std::vector<std::pair<Point, double>> classify(Point test_point, std::vector<Point> learning_set, unsigned int learning_set_count, unsigned int K)
{
	std::vector<std::pair<Point, double>> distances;
	distances.reserve(learning_set_count);
	for (const auto j : learning_set)
	{
		auto d = j.distance(test_point);
		distances.push_back({ j, d });
	}
	std::sort(distances.begin(), distances.end(), [](auto &l, auto &r) {
		return l.second < r.second;
	});
	return std::vector<std::pair<Point, double>>(distances.begin(), distances.begin()+K);
}

int main()
{
	omp_lock_t lock;
	omp_init_lock(&lock);

	unsigned int K, learning_set_count, test_set_count;
	std::cin >> K;
	std::cin >> learning_set_count;

	std::vector<Point> learning_set;
	learning_set.reserve(learning_set_count);
	initialize_learning_set(learning_set, learning_set_count);
	
	std::cin >> test_set_count;

	std::vector<Point> test_set;
	test_set.reserve(test_set_count);
	initialize_test_set(test_set, test_set_count);

	//for (auto &i : test_set)

#pragma omp parallel for
	for (int it = 0; it < test_set.size(); ++it)
	{
		auto i = &test_set[it];
		auto nearest_neighbours = classify(*i, learning_set, learning_set_count, K);
		unsigned int label[4] = {0, 0, 0, 0};
		for (const auto &n : nearest_neighbours)
		{
			label[n.first.type-1]++;
		}
		auto temp = 0U, type = 0U;
		for(unsigned t = 0; t < 4; ++t)
		{
			if (label[t] > temp) 
			{
				temp = label[t];
				type = t+1;
			}
		}
		(*i).type = type;
		//omp_set_lock(&lock);
		//std::cout << (*i).to_string() << std::endl;
		//omp_unset_lock(&lock);
	}
	return 0;
}