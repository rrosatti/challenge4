#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <string.h>

struct node {
	struct node *prev;
	struct node *next;
	int n;
} *list;

struct utsname unameData; // used to get the kernel version
struct sysinfo sysinfoData;


float getTotalMemory();

int main() {

	printf("PID: %d \n", getpid()); // ID of the calling process
	printf("PPID: %d \n", getppid()); // ID of the parent of the calling process
	
	uname(&unameData);
	printf("Linux Kernel version: %s\n", unameData.version); // display Linux kernel version
	
	sysinfo(&sysinfoData);
	printf("Memory size: %lu\n", sysinfoData.totalram);
	//getTotalMemory();

	return 0;

}


float getTotalMemory() {
/**
	FILE *f;
	int memSize = 0;
	char * line = NULL;
	size_t len = 0;
		

	// open the meminfo file
	f = fopen("/proc/meminfo", "r");
	if (f == NULL) {
		printf("Failed to open file.\n");
		return 0;
	}

	getline(&line, &len, f); // read the first line

	printf("%s\n", line);
	if (1 == sscanf(line, "%[^0123456789]%s", &memSize)) {
		printf("%d\n", memSize);
	}
	
	fclose(f);

	return 1;
*/
}
