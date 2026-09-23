#include <iostream>

using namespace std;

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void bubble_sort(int* p, int Length)
{
	for(int i=0;i<Length;i++)
		for (int j = 0; j < Length - i-1; j++)
		{
			if (p[j] < p[j + 1])
				swap(&p[j], &p[j + 1]);
		}
}