#include <iostream>
#include <omp.h>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <iterator>
#include "omptl/omptl_algorithm"
//#include <Windows.h>

struct Point
{
	int *xi;
	unsigned int type, dimension;
	double dist;
	Point(int *_x, int _d, int _t, double _dist = 0.0): xi(_x), type(_t), dimension(_d), dist(_dist)
	{}
	Point(int _d, int _t, double _dist = 0.0) : type(_t), dimension(_d), dist(_dist)
	{
		xi = new int[dimension];
	}
	Point()
	{
		dimension = 2; xi = new int[dimension]; type = 0; dist = 0.0;
	}
	~Point()
	{
		delete[] xi;
	}
	void distance(Point& other)
	{
		for(size_t i = 0; i < dimension; ++i)
		{
			dist += pow(other.xi[i] - this->xi[i], 2);
		}
		dist = sqrt(dist);
	}

	bool operator< (const Point& other) const
	{
		return (dist < other.dist);
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

/*int cmpfunc(const void* _a, const void* _b)
{
	const Point *a = (Point*) _a;
	const Point *b = (Point*) _b;
	if (a->dist == b->dist)
		return 0;
	if (a->dist > b->dist)
		return 1;
	return -1;
}*/

void initialize_learning_set(Point *learning_vector, unsigned int learning_set_count, unsigned int dim)
{
	for (unsigned int i = 0; i < learning_set_count; ++i)
	{
		int xi, t;
		std::cin >> t;
		Point temp = Point(dim, t);
		for(unsigned int j = 0; j < dim; ++j)
		{
			std::cin >> xi;
			temp.xi[j] = xi;
		}
		learning_vector[i] = temp;
	}
}

void initialize_test_set(Point *test_vector, unsigned int test_set_count, unsigned int dim)
{
	for (unsigned int i = 0; i < test_set_count; ++i)
	{
		int xi;
		Point temp = Point(dim, 0);
		for (unsigned int j = 0; j < dim; ++j)
		{
			std::cin >> xi;
			temp.xi[j] = xi;
		}
		test_vector[i] = temp;
	}
}

bool compare_points(Point a, Point b)
{
	return (a.dist < b.dist);
}

void classify(Point &test_point, Point *learning_set, unsigned int learning_set_count)
{
	//std::pair<Point, double> *distances = new std::pair<Point, double>[learning_set_count];
#pragma omp parallel for shared(learning_set)
	for(int it = 0; it < learning_set_count; ++it)
	{
		learning_set[it].distance(test_point);
		//distances[it] = { learning_set[it], d };
	}
	std::sort(learning_set, learning_set + learning_set_count);
	//qsort(learning_set, learning_set_count, sizeof(Point), cmpfunc);
	//return distances;
}


int main()
{
	omp_lock_t lock;
	omp_init_lock(&lock);


	Point *learning_set, *test_set;
	
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

	learning_set = new Point[learning_set_count];
	initialize_learning_set(learning_set, learning_set_count, dimensions);
	
	std::cin >> test_set_count;

	test_set = new Point[test_set_count];
	initialize_test_set(test_set, test_set_count, dimensions);	
	/*
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	if (::QueryPerformanceCounter(&start) == FALSE)
		throw "foo";
		*/

//#pragma omp parallel for shared(learning_set) num_threads(2)
		for (int it = 0; it < test_set_count; ++it)
		{
			//Point *learning_set_for_thread;
			Point *i = &test_set[it];
			//learning_set_for_thread = new Point[learning_set_count];
			//std::memcpy(learning_set_for_thread, learning_set, sizeof(Point));
			
			////std::copy(std::begin(learning_set), std::end(learning_set), std::begin(learning_set_for_thread));
			classify(*i, learning_set/*_for_thread*/, learning_set_count);

			std::map<unsigned int, unsigned int> labels;

			for (int n = 0; n < K; ++n)
			{
				labels[learning_set/*_for_thread*/[n].type];
				labels[learning_set/*_for_thread*/[n].type]++;
			}
			unsigned int type = max_element(labels);

			(*i).type = type;
			//delete[] learning_set_for_thread;

			//omp_set_lock(&lock); 	
			//std::cout << "point type " << type << std::endl;
			//omp_unset_lock(&lock);
		}
	/*
	if (::QueryPerformanceCounter(&end) == FALSE)
		throw "foo";
	double interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	std::cout << interval << std::endl;
	*/
	delete[] learning_set;
	delete[] test_set;
	omp_destroy_lock(&lock);
	return 0;
}