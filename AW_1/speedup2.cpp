#include <iostream>
#include <omp.h>
#include <sstream>
#include <cmath>
#include <cstring>
#include <map>
#include <algorithm>
#include <iterator>
#include <vector>
//#include <Windows.h>

#define DEBUG
#undef DEBUG
omp_lock_t lock;

struct Point
{
	int x, y, z;
	unsigned int type, dimension;
	double dist;
	Point(int _x, int _y, int _z, int _d, int _t, double _dist = 0.0) : x(_x), y(_y), z(_z), type(_t), dimension(_d), dist(_dist)
	{}
	Point(int _d, int _t) : type(_t), dimension(_d)
	{
		x = y = z = 0;
		dist = 0.0;
	}
	Point()
	{
		dimension = 2; x = y = z = 0; type = 0; dist = 0.0;
	}
	Point(const Point &obj)
	{
		this->type = obj.type;
		this->dimension = obj.dimension;
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
		this->dist = obj.dist;
	}
	void distance(Point& other)
	{
		dist = sqrt(pow(other.x - this->x, 2) + pow(other.x - this->x, 2) + pow(other.x - this->x, 2));
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


unsigned int max_element(std::map<unsigned int, unsigned int> &container)
{
	std::map<unsigned int, unsigned int>::const_iterator it = container.begin();
	unsigned int temp = it->second, temp_key = it->first;

	for (unsigned int i = 1; i < container.size(); ++i)
	{
		if ((++it)->second > temp)
		{
			temp = it->second;
			temp_key = it->first;
		}
	}
	return temp_key;
}

void initialize_learning_set(Point *learning_vector, unsigned int learning_set_count, unsigned int dim)
{
	for (unsigned int i = 0; i < learning_set_count; ++i)
	{
		int x, y, z, t;
		std::cin >> t;
		Point temp = Point(dim, t);

		std::cin >> x >> y >> z;
		temp.x = x; temp.y = y; temp.z = z;

		learning_vector[i] = temp;
	}
}

void initialize_test_set(Point *test_vector, unsigned int test_set_count, unsigned int dim)
{
	for (unsigned int i = 0; i < test_set_count; ++i)
	{
		int x, y, z;
		Point temp = Point(dim, 0);
		std::cin >> x >> y >> z;
		temp.x = x; temp.y = y; temp.z = z;

		test_vector[i] = temp;
	}
}

void classify(Point &test_point, Point *learning_set, unsigned int learning_set_count)
{
	for (int it = 0; it < learning_set_count; ++it)
	{
		learning_set[it].distance(test_point);
	}
	std::sort(learning_set, learning_set + learning_set_count);

#ifdef DEBUG
	omp_set_lock(&lock);
	std::cout << "distance from: " << test_point.str() << std::endl;
	for (int it = 0; it < learning_set_count; ++it)
	{
		std::cout << "to: " << learning_set[it].str() << " is: " << learning_set[it].dist << std::endl;
	}
	omp_unset_lock(&lock);
#endif
}


int main()
{
	omp_init_lock(&lock);

	Point *learning_set, *test_set, *learning_set_for_thread;

	/*LARGE_INTEGER frequency;
	if (::QueryPerformanceFrequency(&frequency) == FALSE)
	throw "foo";
	*/


	std::cin.tie(NULL);
	std::ios_base::sync_with_stdio(false);

	unsigned int K, learning_set_count, test_set_count, dimensions;
	std::cin >> K;
	std::cin >> dimensions;
	std::cin >> learning_set_count;

	learning_set = static_cast<Point*>(_mm_malloc(learning_set_count * sizeof(Point), 64));
	initialize_learning_set(learning_set, learning_set_count, dimensions);

	std::cin >> test_set_count;

	test_set = static_cast<Point*>(_mm_malloc(test_set_count * sizeof(Point), 64));
	initialize_test_set(test_set, test_set_count, dimensions);
	/*
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	if (::QueryPerformanceCounter(&start) == FALSE)
	throw "foo";
	*/
#pragma omp parallel for shared(learning_set) private(learning_set_for_thread)
	for (int it = 0; it < test_set_count; ++it)
	{
		Point *i = &test_set[it];
		learning_set_for_thread = static_cast<Point*>(_mm_malloc(learning_set_count * sizeof(Point), 64));

		memcpy(learning_set_for_thread, learning_set, learning_set_count * sizeof(Point));

		classify(*i, learning_set_for_thread, learning_set_count);

		std::map<unsigned int, unsigned int> labels;

		for (int n = 0; n < K; ++n)
		{
			labels[learning_set_for_thread[n].type];
			labels[learning_set_for_thread[n].type]++;
		}
		unsigned int type;
		if (learning_set_for_thread[0].dist == 0)
			type = learning_set_for_thread[0].type;
		else
			type = max_element(labels);

		(*i).type = type;
		_mm_free(learning_set_for_thread);
#ifdef DEBUG
		omp_set_lock(&lock);
		std::cout << i->str() << std::endl << std::endl;
		omp_unset_lock(&lock);
#endif
	}
	/*
	if (::QueryPerformanceCounter(&end) == FALSE)
	throw "foo";
	double interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	std::cout << interval << std::endl;
	*/

	_mm_free(learning_set);
	_mm_free(test_set);

	omp_destroy_lock(&lock);
	return 0;
}