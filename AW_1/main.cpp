#include <iostream>
#include <omp.h>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <iterator>
//#include <Windows.h>

struct Point
{
	int x, y, type;
	double dist;
	Point(int _x, int _y, int _t, double _d = 0.0): x(_x), y(_y), type(_t), dist(_d)
	{}
	Point()
	{
		x = 0; y = 0; type = 0; dist = 0.0;
	}

	void distance(Point& other)
	{
		dist = sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2));
	}

	std::string to_string() const
	{
		std::stringstream ss;
		ss << "x: " << x << ", y: " << y << ", type: " << type;
		return ss.str();
	}

	bool operator< (const Point& other) const
	{
		return (dist < other.dist);
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

void initialize_learning_set(Point *learning_vector, unsigned int learning_set_count)
{
	for (unsigned int i = 0; i < learning_set_count; ++i)
	{
		int x, y, t;
		std::cin >> x >> y >> t;
		learning_vector[i] = Point(x, y, t);
	}
}

void initialize_test_set(Point *test_vector, unsigned int test_set_count)
{
	for (unsigned int i = 0; i < test_set_count; ++i)
	{
		int x, y;
		std::cin >> x >> y;
		test_vector[i] = Point(x, y, 0);
	}
}

bool compare_points(Point a, Point b)
{
	return (a.dist < b.dist);
}

void classify(Point &test_point, Point *learning_set, unsigned int learning_set_count)
{
	//std::pair<Point, double> *distances = new std::pair<Point, double>[learning_set_count];
	for(unsigned int it = 0; it < learning_set_count; ++it)
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


	Point *learning_set, *test_set, *learning_set_for_thread;
	
	/*LARGE_INTEGER frequency;
	if (::QueryPerformanceFrequency(&frequency) == FALSE)
		throw "foo";
		*/



	std::cin.tie(NULL);
	std::ios_base::sync_with_stdio(false);

	unsigned int K, learning_set_count, test_set_count;
	std::cin >> K;
	std::cin >> learning_set_count;

	learning_set = new Point[learning_set_count];
	initialize_learning_set(learning_set, learning_set_count);
	
	std::cin >> test_set_count;

	test_set = new Point[test_set_count];
	initialize_test_set(test_set, test_set_count);	
	/*
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	if (::QueryPerformanceCounter(&start) == FALSE)
		throw "foo";
		*/

#pragma omp parallel for private(learning_set_for_thread) shared(learning_set)
		for (int it = 0; it < test_set_count; ++it)
		{
			Point *i = &test_set[it];
			learning_set_for_thread = new Point[learning_set_count];
			std::memcpy(learning_set_for_thread, learning_set, sizeof(Point));
			//std::copy(std::begin(learning_set), std::end(learning_set), std::begin(learning_set_for_thread));
			classify(*i, learning_set_for_thread, learning_set_count);

			std::map<unsigned int, unsigned int> labels;

			for (int n = 0; n < K; ++n)
			{
				labels[learning_set_for_thread[n].type];
				labels[learning_set_for_thread[n].type]++;
			}
			unsigned int type = max_element(labels);

			(*i).type = type;
			delete[] learning_set_for_thread;
			//omp_set_lock(&lock); 		
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
	return 0;
}