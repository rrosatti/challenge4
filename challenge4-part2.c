/**

	Remember to put "-lnuma" during the compilation: "gcc ... -lnuma"

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
} *list, *temp, *head;

struct utsname unameData; // used to get the kernel version
struct sysinfo sysinfoData;

void showSystemInfo();
void insert(uint32_t);
void create();
void print();
bool search(uint32_t);
void iniArray();

int numValues = 0;
uint64_t *array;

int main() {

	showSystemInfo();
	
	char c[5] = {'\0'};
	uint32_t value;
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
	iniArray();

	return 0;

}

void showSystemInfo() {
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

void insert(uint32_t value) {
	
	if (list == NULL) {
		create();
		temp->n = value;
		list = temp;
		head = list;
		return;
	} 

	if (search(value))
		return;
	else {
		create();
		temp->n = value;
		head->next = temp;
		temp->prev = head;
		head = temp;
	}
	numValues++;
	
}

void create() {
	temp = (struct node*) malloc(1*sizeof(struct node*));
	temp->prev = NULL;
	temp->next = NULL;
	temp->count = 1;
}

void print() {
	struct node* temp2 = list;
	
	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return;
	}
	
	printf("Values: ");
	while (temp2 != NULL) {
		printf("%" PRIu32 " ", temp2->n);
		temp2 = temp2->next;
	}
	printf("\n");
	free(temp2);
	
}

bool search(uint32_t value) {
	
	struct node* temp2 = list;

	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return false;
	}

	while (temp2 != NULL) {
		if (temp2->n == value) {
			temp2->count++;
			return true;
		}
		temp2 = temp2->next;
	}
	
	return false;

}

void iniArray() {
	struct node *temp2 = list;
	array = malloc(numValues * sizeof(*array));
	
	int i = 0;
	while (temp2 != NULL) {
		array[i] = (((uint64_t) temp2->n) << 32) | ((uint64_t) temp2->count);
		
		printf("%" PRIu64 " \n", array[i]);
		//printf("High: %" PRIu64 " \n", array[i] >> 32);
		temp2 = temp2->next;
		i++;
	}
	
	free(temp2);

}
