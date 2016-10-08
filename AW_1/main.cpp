#include <iostream>
#include <omp.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
//#include <algorithm>
#include <map>

struct Point
{
	int x, y, type;
	Point(int _x, int _y, int _t): x(_x), y(_y), type(_t)
	{}
	Point()
	{
		x = 0; y = 0; type = 0;
	}

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

unsigned int max_element(std::map<unsigned int, unsigned int> &container)
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

void sort_pair_vector(std::vector<std::pair<Point, double> > &pairs)
{
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
}

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

std::vector<std::pair<Point, double> > classify(Point &test_point, std::vector<Point> &learning_set, unsigned int learning_set_count, unsigned int K)
{
	std::vector<std::pair<Point, double> > distances;
	distances.reserve(learning_set_count);
	for (std::vector<Point>::iterator it = learning_set.begin(); it != learning_set.end(); ++it)
	{
		double d = (*it).distance(test_point);
		distances.push_back({ *it, d });
	}

	//std::sort(distances.begin(), distances.end(), [](auto &l, auto &r) {
	//	return l.second < r.second;
	//});
	sort_pair_vector(distances);
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

	//for (auto &i : test_set)

#pragma omp parallel for
//#pragma omp single
	for (unsigned int it = 0; it < test_set.size(); ++it)
	{
		Point *i = &test_set[it];
//#pragma omp task firstprivate(i)
		std::vector<std::pair<Point, double> > nearest_neighbours = classify(*i, learning_set, learning_set_count, K);
//#pragma omp taskwait
		std::map<unsigned int, unsigned int> labels;
		for (std::vector<std::pair<Point, double> >::const_iterator n = nearest_neighbours.begin(); n != nearest_neighbours.end(); ++n)
		//for (const auto &n : nearest_neighbours)
		{
			labels[n->first.type];
			labels[n->first.type]++;
		}
		unsigned int type = max_element(labels);
		/*auto temp = 0U, type = 0U;
		for(unsigned t = 0; t < 4; ++t)
		{
			if (labels[t] > temp) 
			{
				temp = labels[t];
				type = t+1;
			}
		}*/
		(*i).type = type;
//#pragma omp 
		//std::cout << (*i).to_string() << std::endl;
		//omp_unset_lock(&lock);
	}
	return 0;
}