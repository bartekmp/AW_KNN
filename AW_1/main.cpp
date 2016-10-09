#include <iostream>
#include <omp.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>

struct Point
{
	int x, y, type;

	Point(int _x, int _y, int _t): x(_x), y(_y), type(_t) {}

	Point() { x = 0; y = 0; type = 0; }

	double distance(Point other) const
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

unsigned int max_element(std::map<unsigned int, unsigned int> container)
{
	std::map<unsigned int, unsigned int>::const_iterator it = container.begin();
	unsigned int temp = it->second;
	for (unsigned int i = 1; i < container.size(); ++i)
	{
		if ((++it)->second > temp)
		{
			temp = it->second;
		}
	}
	return temp;
}

std::vector<std::pair<Point, double> > sort_pair_vector(std::vector<std::pair<Point, double> > pairs)
{
	std::vector<std::pair<Point, double> > pairs_temp = pairs;
	std::pair<Point, double> temp;
	for (unsigned int j = pairs.size(); j > 1; --j)
		for (unsigned int i = 0; i < pairs.size() - 1; ++i)
		{
			if (pairs[i].second > pairs[i + 1].second)
			{
				temp = pairs[i];
				pairs[i] = pairs[i + 1];
				pairs[i + 1] = temp;
			}
		}
	return pairs_temp;
}
void initialize_learning_set(std::vector<Point> learning_vector, unsigned int learning_set_count)
{
	for (unsigned int i = 0; i < learning_set_count; ++i)
	{
		int x, y, t;
		std::cin >> x >> y >> t;
		learning_vector.push_back(Point(x, y, t));
	}

}

void initialize_test_set(std::vector<Point> test_vector, unsigned int test_set_count)
{
	for (unsigned int i = 0; i < test_set_count; ++i)
	{
		int x, y;
		std::cin >> x >> y;
		test_vector.push_back(Point(x, y, 0));
	}
}

std::vector<std::pair<Point, double> > classify(Point test_point, std::vector<Point> learning_set, unsigned int learning_set_count, unsigned int K)
{
	std::vector<std::pair<Point, double> > distances;
	distances.reserve(learning_set_count);
	for(std::vector<Point>::const_iterator j = learning_set.begin(); j != learning_set.end(); ++j)
	{
		double d = j->distance(test_point);
		distances.push_back({ *j, d });
	}

	distances = sort_pair_vector(distances);
	return std::vector<std::pair<Point, double> >(distances.begin(), distances.begin()+K);
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

#pragma omp parallel for
	for (int it = 0; it < test_set.size(); ++it)
	{
		Point *i = &test_set[it];
		std::vector<std::pair<Point, double> > nearest_neighbours = classify(*i, learning_set, learning_set_count, K);
		std::map<unsigned int, unsigned int> labels;
		for (std::vector<std::pair<Point, double> >::const_iterator n = nearest_neighbours.begin(); n != nearest_neighbours.end(); ++n)
		{
			labels[n->first.type];
			labels[n->first.type]++;
		}
		unsigned int type = max_element(labels);
		(*i).type = type;

		//omp_set_lock(&lock);
		//std::cout << (*i).to_string() << std::endl;
		//omp_unset_lock(&lock);
	}
	return 0;
}