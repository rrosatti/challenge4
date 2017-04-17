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
} *list, *temp, *head, *middle;

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
	
	if (list == NULL) {
		create();
		temp->n = value;
		list = temp;
		head = list; 
		middle = list;
		return;
	} 

	if (search(value) != -1)
		return;
	else {
		create();
		temp->n = value;
		head->next = temp;
		temp->prev = head;
		head = temp; // head will always point to the last added element
	}
	numValues++;
	set_middle();
	
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
		printf("%" PRId32 " ", temp2->n);
		temp2 = temp2->next;
	}
	printf("\n");
	free(temp2);
	
}

int search(int32_t value) {
	
	struct node* temp2 = list;

	if (temp2 == NULL) {
		printf("The list is empty!\n");
		return -1;
	}
	
	int i = 0;
	while (temp2 != NULL) {
		i++;
		if (temp2->n == value) {
			temp2->count++;
			return i;
		}
		temp2 = temp2->next;
	}
	
	return -1;

}

void ini_array() {
	struct node *temp2 = list;
	array = malloc(numValues * sizeof(*array));
	
	int i = 0;
	while (temp2 != NULL) {
		// "n" will be the first 32-bit (HIGH) and "count" will be the last 32-bit (LOW)
		array[i] = (((int64_t) temp2->n) << 32) | ((int64_t) temp2->count);
		
		//printf("High: %" PRId32 " \n", getHigh(array[i]));
		//printf("Low: %" PRId32 " \n", getLow(array[i]));
		temp2 = temp2->next;
		i++;
	}
	
	free(temp2);

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
	struct node *temp2 = list;

	if (list == NULL) {
		printf("The list is empty!\n");
		return;
	}
	
	for (int i = 0; i < (int)(numValues/2); i++) {
		temp2 = temp2->next;
	}

	middle = temp2;

}
