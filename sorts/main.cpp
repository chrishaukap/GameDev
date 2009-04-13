#include "stdio.h"

#include "string.h"



void swap(char& p1, char& p2)

{

   char tmp = p1;

   p1 = p2;

   p2 = tmp;

}

void swap(int& p1, int& p2)

{

   int tmp = p1;

   p1 = p2;

   p2 = tmp;

}



//must be null terminated

void revstr(char* str)

{

   if(!str || !*str)

      return;



   int sz = strlen(str);

   char* p1 = str;

   char* p2 = &str[sz-1];

   while(p1 < p2)

   {

      swap(*p1, *p2);

      ++p1, --p2;

   }

}



void ReverseCharacters(char* str)

{

   fprintf(stderr, "ReverseCharacters: %s\n", str);

   revstr(str);

   fprintf(stderr, "After: %s\n", str);

}



void SpacesToNullTerminators(char* str)

{

   if(str && *str)

   {

      while(*str != '\0')

      {

         if(*str == ' ')

            *str = '\0';

         ++str;

      }

   }

}

void NullTerminatorsToSpaces(char* str, int size)

{

   if(str && *str)

   {

      for(int i=0; i<size; ++i)

      {

         if(str[i] == '\0')

            str[i] = ' ';

      }   

   }

}



void ReverseWordOrder(char* str)

{

   fprintf(stderr, "ReverseWordOrder: %s\n", str);



   if(str && *str)

   {

      int size = strlen(str);

      revstr(str);

      SpacesToNullTerminators(str);



      char* token = str;

      for(int i=0; i < size + 1; ++i) // +1 because we want to hit the final token

      {

         if(str[i] == '\0')

         {

            revstr(token);

            token = &(str[i+1]);

         }         

      }



      NullTerminatorsToSpaces(str, size);

   }

    

   fprintf(stderr, "After: %s\n", str);

}



void StringReversalTests()

{

   {

      char *p = NULL;

      char str1[] = "";

      char str2[] = "blah stupid retarded";

      char str3[] = "blah stupid";

      char str4[] = "blah";

      ReverseCharacters(p);

      ReverseCharacters(str1);

      ReverseCharacters(str2);

      ReverseCharacters(str3);

      ReverseCharacters(str4);

   }

   {

      char *p = NULL;

      char str1[] = "";

      char str2[] = "blah stupid retarded";

      char str3[] = "blah stupid";

      char str4[] = "blah";

      ReverseWordOrder(p);

      ReverseWordOrder(str1);

      ReverseWordOrder(str2);

      ReverseWordOrder(str3);

      ReverseWordOrder(str4);

   }

}



int Partition(int* intArray, 

              int leftIndex, int rightIndex, 

              int pivotIndex)

{

   int pivotValue = intArray[pivotIndex];

   swap(intArray[pivotIndex], intArray[rightIndex]);  // Move pivot to end



   int storeIndex = leftIndex;

   for( int i=leftIndex; i<rightIndex; ++i )

   {

      if(intArray[i] <=pivotValue)

      {

         swap(intArray[i], intArray[storeIndex]);

         storeIndex = storeIndex+1;

      }

   }

   swap(intArray[storeIndex], intArray[rightIndex]);       // Move pivot to its final place

   return storeIndex;

}

void QuickSortRec(int* intArray, int leftIndex, int rightIndex)

{

   if(rightIndex > leftIndex)

   {

      int pivotIndex = leftIndex;

      int pivotNewIndex = Partition(intArray, leftIndex, rightIndex, pivotIndex);

      QuickSortRec(intArray, leftIndex, pivotNewIndex - 1);

      QuickSortRec(intArray, pivotNewIndex +1, rightIndex);

   }

}

void QuickSort(int* intArray, int size)

{

   QuickSortRec(intArray, 0, size-1);

}



void PrintIntArray(int* theArray, int size)

{

   for( int i=0; i<10; ++i )

      fprintf(stderr, "%d ", theArray[i]);

}

void QuickSortTests()

{

   int array1[10] = {3,5,66,9,4,3,55,98,45,23};

   fprintf(stderr, "QuickSort Test1: " );

   PrintIntArray(array1, 10);

   fprintf(stderr, "\n");



   QuickSort(array1, 10);

   PrintIntArray(array1, 10);

   fprintf(stderr, "\n");

}



char* myitoa(int val, int base){

	

	static char buf[32] = {0};	

	int i = 30;	

	for(; val && i ; --i, val /= base)	

		buf[i] = "0123456789abcdef"[val % base];	

	return &buf[i+1];

	

}



void main()

{

   //StringReversalTests();

   //QuickSortTests();



   char* buf = myitoa(65458, 10);

   char* buf2 = myitoa(5465646, 10);

}