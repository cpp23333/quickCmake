
	#include <stdio.h>
	#include <iostream>
	#include "Sum.h"
	using namespace std;
	int main()
	{
	    int a=0, b=0, c=0;
	
	    //------c语言模式，不需要使用库iostream-------
	    //printf("please input two paramerer: ")
	    //scanf("%d", &a)
	    //scanf("%d", &b)
	    //c = Calculate_sum_Of_Two_Number(a,b);
	    //printf("the sum is: %d", c)
	
	    //------ c++模式，需要使用库iostream----------
	    cout<<"please input two paramerer: "<<endl;
	    cin>> a >> b;
	    c = Calculate_sum_Of_Two_Number(a,b);
	    cout<<"the sum is: "<< c <<endl;
	
	    return 0;
	}
