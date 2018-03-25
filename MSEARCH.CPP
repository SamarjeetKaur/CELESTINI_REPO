#include<iostream.h>
#include<conio.h>

/*
for the IIT-Delhi people :-After trying the method shown here, I tried using a double binary search method(to reduce the time complexity
to O(logn)) in which I would find the element on the left diagonal first and then binary search it in the  but that method failed because 
my assumption of there being an equipotential plane parallel to the right diagonal was wrong(basically the row elements increase
in value at the same rate as the column elements, therefore the right diagonal would form a roughly equipotential plane).

the logic shown here is for a 5x5 matrix but it is flexible and can be applied to a matrix of any size.

LOGIC - When we search for something (in a sorted list), we basically do a comparison. We generally have 3 conditions,
1)if the item is found ->print it or whatever
2)if the item is greater than the current element ->do something
3)if the item is less than the current element ->do something else

ALGORITHM-
start from either the top right corner or the bottom left corner(the given code uses the latter). 
while we haven't hit the row or column end point
	1)if item is found 
		celebrate. return whatever and print"element found"
	2)if item is smaller than current element 
		go upwards in the column(row index--)
	3)if item is greater than the current element
		go forwards in the row(column index++)

In a 2d sorted array, the element value increases in both down the column direction and forwards in the row direction,
therefore if we start from either the top left or bottom right corner, if we hit a point where the item is less than the current 
element, we can't decide whether we should go backwards in row or upwards in the column. Similarly, if we hit a point where the item
is greater than the current element, we can't decide whether we should go down the column or forwards in the row. Therefore, it makes 
sense to start from a point where the either one of the row or the column index is at the maximum and the other is at the minimum 
(top right or bottom left corner) so that we can increase one index and decrease the other to traverse through the entire array. 
*/

int  matrix_search(int a[5][5], int row , int col,int item)
{
	int r,c;
	r=row-1; //coordinates of bottom left corner 
	c=0;
	while((c<col)&&(r>=0))
	{
		if(item==a[r][c])
		{
			cout<<"element found at row "<<r+1<<" column "<<c+1;
			return 1;//return 1 for true.
		}
		else if (item<a[r][c])//item is smaller than current element.
		{
			r--;//go up the column
		}
		else//item is larger than the current element
		{
			c++;//go forwards in the row.
		}
	}
	cout<<"element not found";//if the element is not found in the while loop, it doesn't exist in the array
	return 0;//return 0 for false
}

void main()
{
	clrscr();
	int item,output;
	//sample matrix-
	int a[5][5] = {{ 1, 4, 7,11,15},
		       { 2, 5, 8,12,19},
		       { 3, 6, 9,16,22},
		       {10,13,14,17,24},
		       {18,21,23,26,30}};
    cout<<"enter the item to be searched - ";
    cin>>item;
    output = matrix_search(a, 5 , 5, item);
    cout<<"\n"<<output;
    getch();
}


