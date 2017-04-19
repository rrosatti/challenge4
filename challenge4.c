/**

	Remember to put "-lnuma" during the compilation: "gcc ... -lnuma"
	Had to install the lib -> "libnuma-dev"
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <numa.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#define get_high(x) (x >> 32)
#define get_low(x) (x & 0xFFFFFFFF)
#define pack_value_and_count(temp2) (((int64_t) temp2->n << 32) | ((int64_t) temp2->count))

struct node {
	struct node *prev;
	struct node *next;
	int32_t n;
	int32_t count;
} *newNode, *middle, *first, *last;

struct utsname unameData; // used to get the kernel version
struct sysinfo sysinfoData; // used to get the total ram size

void show_system_info();
void get_user_input();
void insert(int32_t);
void create();
void print();
int search(int32_t);
void ini_array();
int32_t my_bin_search(int32_t value, void *data, int type);
void set_middle();
void get_user_search_input();
void show_size_of_structures();

int numValues = 0;
int64_t *myArray;

int main() {

	show_system_info();
	get_user_input();
	
	print();
	ini_array();
	
	get_user_search_input();	
	
	show_size_of_structures();

	return 0;

}

void show_system_info() {
	printf("--------------------\n\n");	

	printf("PID: %d \n", getpid()); // ID of the calling process
	printf("PPID: %d \n", getppid()); // ID of the parent of the calling process
	
	uname(&unameData);
	printf("Linux Kernel version: %s\n", unameData.release); // display Linux kernel version
	
	sysinfo(&sysinfoData);
	printf("Memory size: %.2f GB\n", (float) sysinfoData.totalram / 1024000000);

	printf("NUMA nodes: %d\n", numa_num_configured_nodes()); // show the number of NUMA nodes	

	printf("\n--------------------\n\n");
}

void get_user_input() {
	char c[5] = {'\0'};
	int32_t value;
	printf("Type numbers: (or double ENTER to quit)\n");
	int i = 0;
	do {
		fgets(c, sizeof (c), stdin); // get value as a char array
	
		// check if the user pressed enter
		// if he/she pressed twice, then exit the loop	
		if (strcmp(c, "\n") == 0) {
			i++;
			continue;
		}
		
		// check if the user typed a number
		if (sscanf (c, "%" SCNd32, &value) == 1) {
			insert(value);
		}
		i = 0;
		
	} while (i != 2);

}

void get_user_search_input() {
	char c[5] = {'\0'};
	int32_t value;
	printf("\nNumber to be searched: (or double ENTER to quit)\n");
	int i = 0;
	do {
		fgets(c, sizeof (c), stdin); // get value as a char array
	
		// check if the user pressed enter
		// if he/she pressed twice, then exit the loop	
		if (strcmp(c, "\n") == 0) {
			i++;
			continue;
		}
		
		// check if the user typed a number
		if (sscanf (c, "%" SCNd32, &value) == 1) {
			my_bin_search(value, myArray, 1);
			my_bin_search(value, middle, 0);
		}
		i = 0;
		
	} while (i != 2);


}

void insert(int32_t value) {
	
	if (first == NULL) {
		create();
		newNode->n = value;
		newNode->next = newNode;
		newNode->prev = newNode;
		first = last = middle = newNode;
	 
		return;
	} 

	if (search(value) != -1)
		return;
	else {
		create();
		struct node *temp2 = first;
		newNode->n = value;

		if (value <= first->n) {
			newNode->next = first;
			newNode->prev = last;
			first->prev = newNode;
			last->next = newNode;
			first = newNode;
	
		} else if (value >= last->n) {
			newNode->next = first;
			newNode->prev = last;
			first->prev = newNode;
			last->next = newNode;
			last = newNode;
		} else {
			while (value > temp2->n) {
				temp2 = temp2->next; 
			}

			newNode->next = temp2;
			newNode->prev = temp2->prev;
			temp2->prev->next = newNode;
			temp2->prev = newNode;

		}	
	
	}
	
	//numValues++;
	set_middle();
	
}

void create() {
	newNode = (struct node*) malloc(1*sizeof(struct node));
	newNode->prev = NULL;
	newNode->next = NULL;
	newNode->count = 1;
	numValues++;
}

void print() {
	
	struct node* temp2 = first;

	
	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return;
	}
	
	printf("Values: ");
	do {
		printf("%" PRId32 " ", temp2->n);
		temp2 = temp2->next;
	} while (temp2 != first);
	
	printf("\n");
	
}

int search(int32_t value) {
	
	struct node *temp2 = first;

	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return -1;
	}
	
	int i = 0;
	do {
		i++;
		if (temp2->n == value) {
			temp2->count++;
			return i;
		}
		temp2 = temp2->next;
	} while (temp2 != last);
	
	return -1;

}

void ini_array() {
	
	struct node *temp2 = first;
	myArray = malloc(numValues * sizeof(*myArray));
	
	int i = 0;
	do {
		// "n" will be the first 32-bit (HIGH) and "count" will be the last 32-bit (LOW)
		myArray[i] = pack_value_and_count(temp2);
		
		//printf("High: %ld\n", get_high(myArray[i]));
		//printf("Low: %ld\n", get_low(myArray[i]));

		temp2 = temp2->next;
		i++;		
	} while (temp2 != first);

}

/**
 -- Try to optmize this using the value of the "current middle" to set the "new middle" 
*/
void set_middle() {
	
	struct node *temp2 = first;

	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return;
	}
	
	for (int i = 0; i < (int)(numValues/2); i++) {
		temp2 = temp2->next;
	}

	middle = temp2;

}

int32_t my_bin_search(int32_t value, void *data, int type) {
	int32_t lBound = 0;
	int32_t uBound = numValues;	
	int32_t index = -1;	
	int32_t mid = (lBound+uBound)/2;
	//clock_t begin = clock();	
	//time_t start, end;
	struct timeval start, end;
	float timeSpent;

	//time(&start);
	gettimeofday(&start, NULL);

	char* sType;
	sType = (type == 1) ? "ARRAY" : "LIST";
	
	if (type == 0) {
		struct node *midElem = data;

		while (lBound <= uBound) {
			if (midElem->n < value) {
				lBound = mid + 1;

				int32_t up = ((lBound + uBound)/2) - mid;
				int i = 0;
				while (i < up) {
					midElem = midElem->next;
					i++;
				}
			} else if (midElem->n == value) {
				printf("(%s) Value: %" PRId32 " at pos: %" PRId32 "\n", sType, midElem->n, mid);
				index = mid;
				break;
				//return mid;
			} else {
				uBound = mid - 1;

				int32_t down = mid - ((lBound + uBound)/2);
				int i = 0;
		
				while (i < down) {
					midElem = midElem->prev;
					i++;
				}
			}
			
			mid = (lBound + uBound)/2;
		}		

	} else if (type == 1) {

		while (lBound <= uBound) {
			if (get_high(((int64_t *) data)[mid]) < value) {
				lBound = mid + 1;
			} else if (get_high(((int64_t *)data)[mid]) == value) {
				printf("(%s) Value: %" PRId32 " at pos: %" PRId32 "\n", sType, value, mid);
				index = mid;
				break;
				//return mid;
			} else {
				uBound = mid - 1;
			}

			mid = (lBound + uBound)/2;
				
		}
	
	}
	
	//clock_t end = clock();
	//double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	//time(&end);
	//timeSpent = difftime(end, start);
	gettimeofday(&end, NULL);

	timeSpent = (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec) / 1000.0f;
	
	printf("(%s) Time spent: %f microseconds.\n", sType, timeSpent);
	
	return index;
}

void show_size_of_structures() {
	size_t sizeArray = numValues * sizeof(int64_t);
	size_t sizeList = numValues * sizeof(struct node);

	printf("Array: %zu bytes.\n", sizeArray);
	printf("List: %zu bytes.\n", sizeList);
}


