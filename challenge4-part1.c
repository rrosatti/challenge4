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
	int32_t n;
} *newNode, *first, *last;

struct utsname unameData; // used to get the kernel version
struct sysinfo sysinfoData;

void show_system_info();
void insert(int32_t);
void create();
void print();
bool search(int32_t);

int count = 0;

int main() {

	show_system_info();
	
	char c[5] = {'\0'};
	int value;
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
		first = last = newNode;
		
		return;
	} 

	if (search(value))
		return;
	else {
		create();
		struct node *temp2 = first;
		newNode->n = value;

		if (value < first->n) {
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
	count++;
	
}

void create() {
	newNode = (struct node*) malloc(1*sizeof(struct node*));
	newNode->prev = NULL;
	newNode->next = NULL;
}

void print() {
	struct node* temp2 = first;
	
	if (first == NULL) {
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

bool search(int32_t value) {
	
	struct node* temp2 = first;

	if (first == NULL) {
		printf("The list is empty!\n");
		return false;
	}

	do {
		if (temp2->n == value)
			return true;
		temp2 = temp2->next;
	} while (temp2 != first);

	return false;

}
