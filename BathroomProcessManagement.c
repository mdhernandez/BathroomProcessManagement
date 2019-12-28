//Author Michael Hernandez
//UIS ID mhern61
//Date last modified 12/7/2019

//Program Description
//Implementation in C of a unisex bathroom system where only one gender can occupy the bathroom at a time
//People who are unable to enter the bathroom wait in a queue until they can enter
//In each cycle, one new person will attempt to enter the bathroom and either succeed or move to the end of the queue
//A person who enters the bathroom will leave the bathroom two cycles later.
//The program has two modes, automatic and manual that determine the amount of user interaction utilized during execution
//At the end of each cycle, information about the statuses of the bathroom and the queue are displayed
//The program runs for 21 cycles in either mode

//Program Purpose
//Abstractly demonstrating how operating systems may manage processes using a status-defining mutex

#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//return 0(true) if a woman can enter the bathroom. Return 1(false) if she cannot enter because either there is a queue or the bathroom is occupied by men
int woman_wants_to_enter( int status,  int count)
{
	//if the bathroom is occupied by males
	if (status == -1 || count > 0)
	{
		return 1;//the female cannot yet enter
	}
	return 0;//the woman can enter
}

//returns 0(true) if a man can enter the bathroom. Return 1(false) if he cannot enter because either there is a queue or the bathroom is occupied by women
int man_wants_to_enter( int status,  int count)
{
	//if the bathroom is occupied by females
	if (status == 1 || count > 0)
	{
		return 1;//the male cannot yet enter
	}
	return 0;// the male can enter
}

//returns the bathroom status after a man leaves it at the end of a cycle
int man_leaves(int menNum)
{
	if (menNum == 0)
	{
		return 0;
	}
	return -1;
}

//returns the bathroom status after a woman leaves it at the end of a cycle
int woman_leaves(int womenNum)
{
	if (womenNum == 0)
	{
		return 0;
	}
	return 1;
}

//displays the bathroom occupancy status
void display_status(int status, int men, int women)
{
	printf("The bathroom is currently ");
	if (status == -1)
	{
		printf("Occupied by %d men\n", men);
	}

	else if (status == 1)
	{
		printf("Occupied by %d women\n", women);
	}

	//if status is 0
	else
	{
		printf("empty\n");
	}
	printf("\n");//formatting
}

//displays the status of the waiting queue
void display_queue(int *queue, int front, int count, int max)
{
	//if the line is currently empty
	if (count == 0)
	{
		printf("The line is currently empty\n\n");
		return;
	}

	int position = 1;//used to display the position of each person in the line

	printf("The line is currently filled with these people:\n");

	//prints the position and gender of each person currently waiting in the line
	int i = front;
	while (i < front + count)
	{
		//only tries to access a position in the queue if it is within its bounds
		if (i < max)
		{
			if (queue[i] == 1)
			{
				printf("Position %d: Female ", position);
			}

			else if (queue[i] == -1)
			{
				printf("Position %d: Male ", position);
			}
		}
		if(queue[i]!=0)
		{
			position = position + 1;
		}
		i = i + 1;
	}
	printf("\n\n");//formatting
}

int main(int argc, const char* argv[])
{
	int bathroomStatus = 0;//represents the occupancy status of the bathroom
	int cycleNumber = 1;//tracks the number of operation cycles for display purposes
	const int maxPeople = 100;//represents the maximum number of people for this simulation
	int nextPerson = 0;//will be used to determine the gender of the next person who wants to use the bathroom
	int menInBathroom = 0;
	int womenInBathroom = 0;
	int canEnter = -1;//determines whether an attempt to enter the bathroom is successful


	//line queue implementation
	int queue[maxPeople];//represents the queue as an Array
	int frontIndex = 0;//stores the index of the array that represents the front of the queue
	int queueCount = 0;//represents the number of people currently in the queue

	//mode selection block
	int mode = 0;//stores the value for the program execution mode chosen by the user

	while (mode != 1 && mode != 2)
	{
		printf("Please enter '1' for automatic or '2' for manual\n");
		//Note* scanf statements are encapsulated in if statements to prevent issues
		//with some compilers requiring the return value of scanf to be used.
		if (scanf("%d", &mode)) {}
		printf("\n");//formatting
	}
	//end mode selection

	//automatic execution mode
	if (mode == 1)
	{
		srand(time(0));//sets seed for random number generator to the current time;

		//the program runs for 21 cycles
		while (cycleNumber <= 21)
		{

			//somebody leaves the bathroom every other cycle if it is occupied
			//
			//if the bathroom is occupied by women
			if (cycleNumber % 2 == 1) 
			{
				if (bathroomStatus == 1)
				{
					womenInBathroom = womenInBathroom - 1;
					bathroomStatus = woman_leaves(womenInBathroom);

				}

				//if the bathroom is occupied by men
				else if (bathroomStatus == -1)
				{
					menInBathroom = menInBathroom - 1;
					bathroomStatus = man_leaves(menInBathroom);
				}
			}
			

			nextPerson = (rand() % 2) + 1;//the gender of the next person is assigned randomly

			//if the nextPerson is female
			if (nextPerson == 1)
			{
				printf("female being called\n");// test
				
					canEnter = woman_wants_to_enter( bathroomStatus, queueCount);
					//if the woman can enter, increase the number of women in the bathroom by 1
					if (canEnter == 0)
					{
						womenInBathroom = womenInBathroom + 1;
						bathroomStatus = 1;
					}

					//if the woman cannot enter she is placed in queue and the queueCount is incremented
					else
					{
						queue[frontIndex + queueCount] = 1;
						queueCount = queueCount + 1;
					}
				
			}

			//if the nextPerson is male
			else
			{
				printf("male being called\n");//test
				
				
					canEnter = man_wants_to_enter(bathroomStatus,  queueCount);

					//if the man can enter, increase the number of men in the bathroom by 1
					if (canEnter == 0)
					{
						menInBathroom = menInBathroom + 1;
						bathroomStatus = -1;
					}
					//if the man cannot enter he is placed in queue and the queueCount is incremented
					else
					{
						queue[frontIndex + queueCount] = -1;
						queueCount = queueCount + 1;
					}
				
			}

			//queue update operations - allowing or disallowing people to enter
			//In automatic mode, if the first person in line is the opposite gender
			//as the bathroom status, nobody enters the bathroom in this cycle
			int temp = 0;//for temporary use in queue operations

			switch (bathroomStatus)
			{
			case 0://bathroom empty
				bathroomStatus = queue[frontIndex];// the next person in queue enters and determines the status of the bathroom
				
				if (bathroomStatus != 0) 
				{
					frontIndex = frontIndex + 1;//the next person in line becomes the front of the line
					queueCount = queueCount - 1;//queue count is decremented
				}
				

				//if the bathroom is now occupied by a woman
				if (bathroomStatus == 1)
				{
					womenInBathroom = womenInBathroom + 1;
				}
				//if the bathroom is now occupied by a man
				else if (bathroomStatus == -1)
				{
					menInBathroom = menInBathroom + 1;
				}

				break;

			case 1://bathroom occupied by women
			//if the first person in line is a woman
				if (queue[frontIndex] == 1)
				{
					womenInBathroom = womenInBathroom + 1;
					frontIndex = frontIndex + 1;
					queueCount = queueCount - 1;
				}
				break;

			case -1://bathroom occupied by men
			//if the first person in line is a man
				if (queue[frontIndex] == -1)
				{
					menInBathroom = menInBathroom + 1;
					frontIndex = frontIndex + 1;
					queueCount = queueCount - 1;
				}
				break;
			}

			//displays status after each cycle
			printf("Cycle %d:\n", cycleNumber);
			display_status(bathroomStatus, menInBathroom, womenInBathroom);
			display_queue(queue, frontIndex, queueCount, maxPeople);
			canEnter = -1;//resets variable
			cycleNumber = cycleNumber + 1;//increments cycle number
		}
	}

	//manual execution mode
	else
	{
		//the program runs for 21 cycles
		while (cycleNumber <= 21)
		{
			//somebody leaves the bathroom every other cycle if it is occupied
			//
			//if the bathroom is occupied by women
			if (cycleNumber % 2 == 1)
			{
				if (bathroomStatus == 1)
				{
					womenInBathroom = womenInBathroom - 1;
					bathroomStatus = woman_leaves(womenInBathroom);

				}

				//if the bathroom is occupied by men
				else if (bathroomStatus == -1)
				{
					menInBathroom = menInBathroom - 1;
					bathroomStatus = man_leaves(menInBathroom);
				}
			}
			//user input loop
			while (nextPerson != 1 && nextPerson != 2)
			{
				printf("Enter '1' to add a female or '2' to add a male\n");
				if (scanf("%d", &nextPerson)) {}
				printf("\n");//formatting
			}

			//if the nextPerson is female
			if (nextPerson == 1)
			{
				canEnter = woman_wants_to_enter( bathroomStatus, queueCount);

				//if the woman can enter, increase the number of women in the bathroom by 1
				if (canEnter == 0)
				{
					womenInBathroom = womenInBathroom + 1;
					bathroomStatus = 1;
				}
				//if the woman cannot enter she is placed in queue and the queueCount is incremented
				else
				{
					queue[frontIndex + queueCount] = 1;
					queueCount = queueCount + 1;
				}
			}

			//if the nextPerson is male
			else
			{
				
				canEnter = man_wants_to_enter(bathroomStatus, queueCount);

				//if the man can enter, increase the number of men in the bathroom by 1
				if (canEnter == 0)
				{
					menInBathroom = menInBathroom + 1;
					bathroomStatus = -1;
				}
				//if the man cannot enter he is placed in queue and the queueCount is incremented
				else
				{
					queue[frontIndex + queueCount] = -1;
					queueCount = queueCount + 1;
				}
			}

			//queue update operations - allowing or disallowing people to enter
			//In manual mode, If the bathroom is occupied, the queue is traversed
			//until a person eligible to enter is found, that person is then allowed to enter
			switch (bathroomStatus)
			{
			case 0://bathroom empty
				bathroomStatus = queue[frontIndex];// the next person in queue enters and determines the status of the bathroom
				
				if (bathroomStatus != 0) 
				{
					frontIndex = frontIndex + 1;//the next person in line becomes the front of the line
					queueCount = queueCount - 1;
				}
				
				//if the bathroom is now occupied by a woman
				if (bathroomStatus == 1)
				{
					womenInBathroom = womenInBathroom + 1;
				}
				//if the bathroom is now occupied by a man
				else if (bathroomStatus == -1)
				{
					menInBathroom = menInBathroom + 1;
				}

				break;

			case 1://bathroom occupied by women
			//if the first person in line is a woman
				if (queue[frontIndex] == 1)
				{
					womenInBathroom = womenInBathroom + 1;
					frontIndex = frontIndex + 1;
					queueCount = queueCount - 1;
				}

				//otherwise we search to see if there is a woman further down the line
				else
				{
					if ((frontIndex + queueCount) < maxPeople)
					{
						int i = frontIndex;
						while ( i < frontIndex + queueCount)
						{
							if (queue[i] == 1)
							{
								womenInBathroom = womenInBathroom + 1;
								queue[i] = 0;//to prevent the spot from being wrongly processed
								break;
							}
							i = i + 1;
						}
					}

				}

				
				break;

			case -1://bathroom occupied by men
			//if the first person in line is a man
				if (queue[frontIndex] == -1)
				{
					menInBathroom = menInBathroom + 1;
					frontIndex = frontIndex + 1;
					queueCount = queueCount - 1;
				}

				//otherwise we search to see if there is a man further down the line
				else
				{
					if ((frontIndex + queueCount) < maxPeople)
					{
						int j = frontIndex;
						while ( j < frontIndex + queueCount)
						{
							if (queue[j] == -1)
							{
								menInBathroom = menInBathroom + 1;
								queue[j] = 0;//to prevent the spot from being wrongly processed
								break;
							}
							j = j + 1;
						}
					}
				}
				break;
			}

			//displays status after each cycle
			printf("Cycle %d:\n", cycleNumber);
			display_status(bathroomStatus, menInBathroom, womenInBathroom);
			display_queue(queue, frontIndex, queueCount, maxPeople);

			nextPerson = 0;//allows input loop to run next cycle
			canEnter = -1;//resets variable
			cycleNumber = cycleNumber + 1;//increments cycle number
		}
	}
}