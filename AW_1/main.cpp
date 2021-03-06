#include <iostream>
#include <omp.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>

struct Point
{
	int x, y, z;
	unsigned int type;
	double dist;
	Point(int _x, int _y, int _z, int _t, double _dist = 0.0) : x(_x), y(_y), z(_z), type(_t), dist(_dist)
	{}
	Point(int _t) : type(_t)
	{
		x = y = z = 0;
		dist = 0.0;
	}
	Point()
	{
		x = y = z = 0; type = 0; dist = 0.0;
	}
	Point(const Point &obj)
	{
		this->type = obj.type;
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
		this->dist = obj.dist;
	}
	double distance(Point& other)
	{
		dist = sqrt(pow(other.x - this->x, 2) + pow(other.x - this->x, 2) + pow(other.x - this->x, 2));
		return dist;
	}

	bool operator< (const Point& other) const
	{
		return dist < other.dist;
	}

	std::string str() const
	{
		std::stringstream ss;
		ss << "x = " << x << ", y = " << y << ", z = " << z << ", type = " << type;
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
int cmpfunc(const void* _a, const void* _b)
{
	const std::pair<Point, double> *a = (std::pair<Point, double>*) _a;
	const std::pair<Point, double> *b = (std::pair<Point, double>*) _b;
	if (a->second == b->second)
		return 0;
	if (a->second > b->second)
		return 1;
	return -1;
}

std::vector<std::pair<Point, double> > sort_pair_vector(std::vector<std::pair<Point, double> > pairs)
{
	std::vector<std::pair<Point, double> > pairs_temp = pairs;
	std::pair<Point, double> temp;
	for (unsigned int j = pairs.size(); j > 1; j--)
		for (unsigned int i = 0; i < pairs.size() - 1; i++)
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
std::vector<Point> initialize_learning_set(unsigned int learning_set_count)
{
	std::vector<Point> tmp;
	for (unsigned int i = 0; i < learning_set_count; i++)
	{
		int x, y, z, t;
		std::cin >> x >> y >> z >> t;
		tmp.push_back(Point(x, y, z, t));
	}
	return tmp;
}

std::vector<Point> initialize_test_set(unsigned int test_set_count)
{
	std::vector<Point> tmp;
	for (unsigned int i = 0; i < test_set_count; i++)
	{
		int x, y, z;
		std::cin >> x >> y >> z;
		tmp.push_back(Point(x, y, z, 0));
	}
	return tmp;
}

std::vector<std::pair<Point, double> > classify(Point test_point, std::vector<Point> learning_set, unsigned int learning_set_count, unsigned int K)
{
	std::vector<std::pair<Point, double> > distances;
	distances.reserve(learning_set_count);
	for(std::vector<Point>::iterator j = learning_set.begin(); j != learning_set.end(); ++j)
	{
		double d = j->distance(test_point);
		distances.push_back({ *j, d });
	}
	qsort(&distances[0], distances.size(), sizeof(std::pair<Point, double>), cmpfunc);
	//distances = sort_pair_vector(distances);
	return std::vector<std::pair<Point, double> >(distances.begin(), distances.begin()+K);
}


int main()
{
	unsigned int K, learning_set_count, test_set_count;
	std::cin >> K;
	std::cin >> learning_set_count;

	std::vector<Point> learning_set = initialize_learning_set(learning_set_count);
	//std::vector<Point> learning_set;
	//learning_set.reserve(learning_set_count);
	//std::copy(tmp_lrnset.begin(), tmp_lrnset.end(), learning_set.begin());
	
	std::cin >> test_set_count;
	
	std::vector<Point> test_set = initialize_learning_set(learning_set_count);
	//std::vector<Point> test_set;
	//learning_set.reserve(test_set_count);
	//std::copy(tmp_testset.begin(), tmp_testset.end(), test_set.begin());

#pragma omp parallel for
	for (int it = 0; it < test_set.size(); ++it)
	{
		Point i = test_set[it];
		std::vector<std::pair<Point, double> > nearest_neighbours = classify(test_set[it], learning_set, learning_set_count, K);
		std::map<unsigned int, unsigned int> labels;
		for (std::vector<std::pair<Point, double> >::iterator n = nearest_neighbours.begin(); n != nearest_neighbours.end(); ++n)
		{
			labels[n->first.type];
			labels[n->first.type]++;
		}
		unsigned int type = max_element(labels);
		test_set[it].type = type;

		//omp_set_lock(&lock);
		//std::cout << (*i).to_string() << std::endl;
		//omp_unset_lock(&lock);
	}
	return 0;
}