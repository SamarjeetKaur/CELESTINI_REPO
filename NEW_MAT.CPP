#include<iostream.h>
#include<conio.h>

/*
The code was written in turboC7 environment and not in C++11, so if you want to run it,
run it in turboC7(no bugs) (or you could add namespace std; and change the header file names
to work with c++11).
NOTE:
The following piece of code is designed for multiplying and convolving 5x5 matrices.
However, the logic, as you shall see, is flexible and can be implemented on matrices 
of any size. The reason why I kept the matrices of fixed size is because otherwise I 
would have to implement nested linked lists which would make the logic very difficult
to understand for the reader. Keeping the matrices of a fixed size allows the reader 
to focus better on the main logic. I assume the reader already knows the logic for 
naive matrix multiplication and convolution( I mean if you don't then what exactly are
you trying to do reading this code? :P).
Also, this logic accepts the generic/standard form of a matrix(no RCS/RCV nonsense) for the
sake of portability(bonus points please). 
*/

//----------------------------LOGIC FOR SPARSE MATRIX MULTIPLICATION------------------
/*
BASIC IDEA - find positions of all the non zero elements in each row of matrix of A and each column of B,
count the number of such elements too! We wanna do as little work as possible, so in each row/column change, 
we wanna compare if the row in mat A has less elements than those in the corresponding column in B. Once that is done,  
simply multiply the elements as you do in the standard method. The counting is required to figure out how many time the 
innermost loop is supposed to run(so that we don't naively run the loop as many times as the rows in B).
*/
struct node //A struct which either represents the row or the column of the matrix
{		//for the first matrix, it would represent the row. Each row will have some non zero elements
	int index[5];//this holds the index value(column index for first matrix) of the non zero elements in the matrix
				//basically in each row(first matrix) I sequentially store the position of the non zero element.
	int size; //hold the number of non zero elements 
			  //(in the row for the first matrix and the column for the second matrix)
};

struct node x[5],y[5];
//x-> 5 rows therefore 5 objects of node
//y-> 5 columns therefore 5 objects of node.

void multiply_sparse(int a[5][5],int b[5][5])
{
	int c[5][5],k,i,j,m,n;
	/*
	The first thing to do is to get the information about 
	where the non zero elements in each matrix are in each 
	row(first matrix) or column and put that information 
	in x and y.
 	*/
 	//first matrix- 
	for(i=0;i<5;i++)//row from 0 to 4
	{
		x[i].size = 0;//for each row, the number of non 
		k = 0;//zero elements is initialized at 0. k is just a counter 
		for(j = 0;j<5;j++)//column from 0 to 4
		{
			if(a[i][j]!=0)//if a non zero element is found
			{
				x[i].index[k] = j;//store it's column index in x[i].index[k]
				k++;//increment k.
				x[i].size = k;//so far we have encountered k non zero elements
							//so the size so far is equal to k.
			}
		}
	}
	//second matrix
	for(j=0;j<5;j++)//column from 0 to 4 
	{
		k=0; //initialize counter
		y[j].size = 0;//initialize size as 0(explanation is same as that for first matrix)
		for(i = 0;i<5;i++)//row from 0 to 4
		{
			if(b[i][j]!=0)//non zero element encountered
			{
				y[j].index[k] = i;//store it's position
				k++;//increment counter
				y[j].size = k;//update the size
			}
		}
	}
	//this process has O(n^2) complexity
	//now that we are done extracting the information we needed
	//lets do the MULTIPLICATION
	for(i=0;i<5;i++)//row traversal
	{
		for(j=0;j<5;j++)//column traversal
		{
			m=0;//remember the sequentially stored position thing(struct node index[5] comment)?
					// Yeah this is a variable to traverse that.
			c[i][j] = 0; //set c[i][j] as 0
			//k is = min( (number of non zeros in a row of A),(number of non-zeros in corresponding column of B))
			//basically, if row in A has 1 non zero element and corresponding column in B has 3 non zero 
			//elements, it is better to just multiply the 1 non zero element in A with it's corresponding element
			//in B than to run the loop 3 times and multiply elements in B with 0 atleast twice. 
			if(x[i].size<y[j].size)//if the number of non zero elements in row of A is less than
			{					//that of the column in B
				k = x[i].size;
				while(k)//run the loop k number of times.(I use a while loop because I felt like it. Bite me) 
				{
					c[i][j] += a[i][(x[i].index[m])]*b[(x[i].index[m])][j];//do the deed.
					m++;//go to the next column index in the row of A
					k--;//decrease k
				}
			}
			if(x[i].size>=y[j].size)//vice-a-versa
			{
				k = y[j].size;
				while(k) 
				{
					c[i][j] += a[i][(y[j].index[m])]*b[(y[j].index[m])][j];//do the deed.
					m++;// go to the next row index in column of B
					k--;
				}
			}
		}
	}
	/*
	//time complexity of the multiplication part is (k*n^2) 
	//where k is 
	for(i=0;i<number of rows;i++)
	{	
		k += min((non zero elemnts in row of A),(non zero elements in ith column of B));
	}
	*/
	// DONE! 
	//the following part is just display.
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			cout<<c[i][j]<<" ";
		}
		cout<<endl;
	}
}

//-----------------------------------------CONVOLUTION-----------------------------------
/*
BASIC IDEA - as with multiplication, I need to know the positions of the non zero elements
as well as how many there are, except that here I don't care about their numbers column-wise
or row-wise. All that matters is how many non zero elements are there in total. This information 
is used to figure out how many times the innermost loop needs to be run.
*/
struct conv_node //a struct that stores the row and column index of the non zero elements in
{				//matrix B.
	int row,column;
}w[25],z[25]; //worst case scenario- all elements in A,B are non-zero.

void conv_sparse(int a[5][5],int b[5][5])
{
	int i,j,k,m,n,d[5][5],c[5][5],count[2]={0,0}
	/*
	I'm doing 2 things here, first off, I m flipping the B(mask) matrix
	around the diagonal and I m storing the row and column indeces of
	non zero elements in a sequential manner for both matrices and counting them too.
	*/
	for(i =0;i<5;i++)//row
	{
		for(j =0;j<5;j++)//column
		{
			d[i][j] = b[4-i][4-j]; //flip the mask around the diagonal
			if(d[i][j]!=0)//non zero element found
			{
				z[count[0]].row = i;//row index
				z[count[0]].column = j;//column index
				count[0]++;//increment count- it is the number of
				//non zero elements in B. it is apparently also
				// the max number of times i need to run the loop.
			}
			if(a[i][j]!=0) //do the same for the first matrix
			{
				w[count[1]].row = i;
				w[count[1]].column = j;
				count[1]++;//increase the count
			}
		}
	}
	//Now that we have obtained the information we need,
	//lets do the actual CONVOLUTION
	for(i =0;i<5;i++)//row
	{
		for(j=0;j<5;j++)//column
		{
			c[i][j] = 0;//set c[i][j] =0
			if(count[0]<=count[1]) //if B matrix has less non zero elements,
			{
				for(k = 0;k<count[0];k++)//run a for loop only as many times as the number
				{					//of non zero elements in B. 
					m = z[k].row;//get the row and column of the non zero element in matrix B 
					n = z[k].column;
					if((m+i<=5&&m+i>=1)&&(n+j<=5&&n+j>=1))//this is just to make sure we stay
					{		//within the bounds of the A matrix (padding->0)
						c[i][j] += a[m+i-1][n+j-1]*d[m][n];//doing the deed.
					}
				}
			}

			else if(count[1]<count[0])//if A matrix has less non zero elements
			{		
				for(k = 0;k<count[1];k++)//run a for loop only as many times as the number
				{					//of non zero elements in A. 
					m = w[k].row;//get the row and column index of the non zero element in matrix A
					n = w[k].column;
					if((m-i<=3&&m-i>=-1)&&(n-j<=3&&n-j>=-1))//just making sure we stay within bounds 
					{			//of matrix B 
						c[i][j] += a[m][n]*d[m+1-i][n+1-j]; //doing the deed
					}
				}
			}

		}
	}
	//DONE!
	/*
	the convolution process takes K*n^2 time where 
	K = min((non zero in A),(non zero in B))
	*/
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			cout<<c[i][j]<<" ";
		}
		cout<<endl;
	}
}


void main()
{
	clrscr();
	//Play around with these matrices-
	int a[5][5] = {{0,0,0,0,0},
		       {0,0,0,9,0},
		       {11,0,13,14,15},
		       {0,17,0,0,20},
		       {21,0,23,24,25}};
	int b[5][5] = {{0,0,3,0,0},
		       {0,7,8,9,0},
		       {11,12,13,14,15},
		       {0,17,18,19,0},
		       {0,0,23,0,0}};
	cout<<"multiplication = "<<endl;
	multiply_sparse(a,b);
	cout<<endl;
	cout<<"convolution = "<<endl;
	conv_sparse(a,b);
	getch();
}