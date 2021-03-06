#include "stdio.h"
#include "quicksort.h"
#include "mergesort.h"
#include "stringReversal.h"
#include "myitoa.h"

void TestReverseWordOrder(char* str)
{
   fprintf(stderr, "BEFORE: [%s]\n", str);
	ReverseWordOrder(str);
   fprintf(stderr, "AFTER: [%s]\n", str);
}
void TestReverseCharacters(char* str)
{	
   fprintf(stderr, "BEFORE: [%s]\n", str);
	ReverseCharacters(str);
   fprintf(stderr, "AFTER: [%s]\n", str);
}

void PrintIntArray(int* theArray, int size)
{
   for( int i=0; i<size; ++i )
      fprintf(stderr, "%d ", theArray[i]);
}

void StringReversalTests()
{
	fprintf(stderr, "Character Reversal Tests\n======================\n");
   {
		char str1[] = "The";
		char str2[] = "The Horse";
		char str3[] = "The cat ate the dog";
		TestReverseCharacters(NULL);
		TestReverseCharacters("");
      TestReverseCharacters(str1);
		TestReverseCharacters(str2);
		TestReverseCharacters(str3);
   }
	fprintf(stderr, "\n");
	fprintf(stderr, "Word Order Reversal Tests\n======================\n");
   {
		char str1[] = "The";
		char str2[] = "The Horse";
		char str3[] = "The cat ate the dog";
		TestReverseWordOrder(NULL);
		TestReverseWordOrder("");
      TestReverseWordOrder(str1);
		TestReverseWordOrder(str2);
		TestReverseWordOrder(str3);
   }
	fprintf(stderr, "\n");
}

void QuickSortTests()
{
	const int size = 24;
   int array1[size] = {3,5,66,9,4,3,55,98,45,23,67,345,78,346,234,47,3456,24,3465,57,85,96,6,3};
	fprintf(stderr, "QUICKSORT TESTS\n===================\n" );
	{
		fprintf(stderr, "BEFORE:" );
		PrintIntArray(array1, size);
		fprintf(stderr, "\n");

		QuickSort<int>(array1, size);

		fprintf(stderr, "AFTER:" );
		PrintIntArray(array1, size);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}
void MergeSortTests()
{
	const int size = 24;
   int array1[size] = {3,5,66,9,4,3,55,98,45,23,67,345,78,346,234,47,3456,24,3465,57,85,96,6,3};
	fprintf(stderr, "MERGESORT TESTS\n===================\n" );
	{
		fprintf(stderr, "BEFORE:" );
		PrintIntArray(array1, size);
		fprintf(stderr, "\n");

		MergeSort(array1, size);

		fprintf(stderr, "AFTER:" );
		PrintIntArray(array1, size);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}
void main()
{
   StringReversalTests();
   QuickSortTests();
	MergeSortTests();

   char* buf = myitoa(65458, 10);
   char* buf2 = myitoa(5465646, 10);
   char* buf3 = myitoa(24, 16);
   char* buf4 = myitoa(12, 16);
   char* buf5 = myitoa(24, 8);
}