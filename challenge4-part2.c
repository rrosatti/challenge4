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

struct node {
	struct node *prev;
	struct node *next;
	uint32_t n;
	uint32_t count;
} *newNode, *middle, *first, *last;

struct utsname unameData; // used to get the kernel version
struct sysinfo sysinfoData;

void show_system_info();
void insert(int32_t);
void create();
void print();
int search(int32_t);
void ini_array();
int32_t get_high(int64_t);
int32_t get_low(int64_t);
int32_t my_bin_search(int32_t value, void *data, int type);
void set_middle();

int numValues = 0;
int64_t *array;

int main() {

	show_system_info();
	
	char c[5] = {'\0'};
	int32_t value;
	printf("Type numbers.\n");
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

	print();
	ini_array();
	printf("Middle: %" PRId32 "\n", middle->n);

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

		// check if the value is equals or less than the 1st element
		// if it is not, then it will search for another position
		newNode->n = value;
		if (value <= first->n) {
			printf("First.\n");
			//first->prev = newNode;
			newNode->next = first;
			newNode->prev = last;
			first->prev = newNode;
			last->next = newNode;
			first = newNode;
	
		} else if (value >= last->n) {
			printf("Last.\n");
			newNode->next = first;
			newNode->prev = last;
			first->prev = newNode;
			last->next = newNode;
			last = newNode;
			printf("Last->next: %" PRId32 "\n", last->next->n);
			printf("Last->prev: %" PRId32 "\n", last->prev->n);
			printf("First->prev: %" PRId32 "\n", first->prev->n);
			printf("First->next: %" PRId32 "\n", first->next->n);

		} else {
			printf("Middle.\n");
			while (value > temp2->n) {
				temp2 = temp2->next; 
			}
			printf("Temp2->n: %" PRId32 "\n", temp2->n);
			printf("Temp2->prev: %" PRId32 "\n", temp2->prev->n);
			printf("Temp2->next: %" PRId32 "\n", temp2->next->n);

			printf("Temp2->prev->next: %" PRId32 "\n", temp2->prev->next->n);
		

			newNode->next = temp2;
			newNode->prev = temp2->prev;
			//temp2->prev = newNode;
			temp2->prev->next = newNode;
			temp2->prev = newNode;

			printf("Temp2->prev: %" PRId32 "\n", temp2->prev->n);
			printf("Temp2->next: %" PRId32 "\n", temp2->next->n);
			printf("First->next: %" PRId32 "\n", first->next->n);
			printf("Temp2->prev->next: %" PRId32 "\n", temp2->prev->next->n);


		}	
	
	}
	
	print();
	printf("first: %" PRId32 "\n", first->n);
	printf("newNode: %" PRId32 "\n", newNode->n);
	printf("last: %" PRId32 "\n", last->n);

	numValues++;
	set_middle();
	
}

void create() {
	newNode = (struct node*) malloc(1*sizeof(struct node*));
	newNode->prev = NULL;
	newNode->next = NULL;
	newNode->count = 1;
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
	//free(temp2);
	
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
	array = malloc(numValues * sizeof(*array));
	
	int i = 0;
	do {
		// "n" will be the first 32-bit (HIGH) and "count" will be the last 32-bit (LOW)
		array[i] = (((int64_t) temp2->n) << 32) | ((int64_t) temp2->count);
		
		printf("High: %" PRId32 " \n", get_high(array[i]));
		printf("Low: %" PRId32 " \n", get_low(array[i]));
		temp2 = temp2->next;
		i++;
	} while (temp2 != first);
	
	//free(temp2);

}

int32_t get_high(int64_t valueAndCount) {
	return valueAndCount >> 32;
}

int32_t get_low(int64_t valueAndCount) {
	return valueAndCount & 0xFFFFFFFF;
}

int32_t my_bin_search(int32_t value, void *data, int type) {
	return -1;
}

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
