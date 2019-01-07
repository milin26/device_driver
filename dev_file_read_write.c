#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_METHOD	"ioctl" 
#define READ_WRITE_CALL "rw"
#define LKM_WR_VALUE	_IOW('.', '.', int32_t*)
#define LKM_RD_VALUE 	_IOR('.', '@', int32_t*)

void ioctl_func(char *file_name);
void rw_func(char *file_name);

int8_t write_buf[1024];
int8_t read_buf[1024];

void ioctl_func(char *file_name)
{
	int fd, d;
	char option;
	int32_t value, number;
	fd = open(file_name, O_RDWR);
	if(fd == 0)
	{
		printf("Cannot open device file...\n");
		close(fd);
		return;
	}
	printf("file_name : %s\n", file_name);
	while(1)
	{
		//fflush();
		printf("****Please Enter the Option******\n");
		printf("        1. Write               \n");
		printf("        2. Read                 \n");
		printf("        3. Exit                 \n");
		printf("*********************************\n");
		scanf(" %c", &option);
		getchar();
		printf("Your Option = %c\n", option);
		switch(option)
		{
			case '1':
				printf("Writing Value to Driver\n");
				printf("Enter the Value to send\n");
				scanf("%d",&number);
				getchar();
				printf("Entered Value is : %d\n", number);
				ioctl(fd, LKM_WR_VALUE, (int32_t *) &number); 
				break;
			case '2':
				printf("Reading Value from Driver\n");
				ioctl(fd, LKM_RD_VALUE, (int32_t *) &value);
				printf("Value is %d\n", value);
				break;
			case '3':
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter Valid option\n");
				break;
		}
	}
	printf("Closing Driver\n");
	close(fd);
	return;
}

void rw_func(char *file_name)
{
	int fd, d;
	char option;
	fd = open(file_name, O_RDWR);
	if(fd < 0) 
	{
		printf("Cannot open device file...\n");
		close(fd);
		return;
	}
	while(1)
	{
		printf("****Please Enter the Option******\n");
		printf("        1. Write               \n");
		printf("        2. Read                 \n");
		printf("        3. Exit                 \n");
		printf("*********************************\n");
		scanf(" %c", &option);
		printf("Your Option = %c\n", option);
		getchar();
		switch(option) 
		{
			case '1':
				printf("Enter the string to write into driver :");
				scanf("  %[^\t\n]s", write_buf);
				getchar();
				printf("Data Writing ...");
				write(fd, write_buf, strlen(write_buf)+1);
				printf("Done!\n");
				break;
			case '2':
				printf("Data Reading ...");
				read(fd, read_buf, 1024);
				printf("Done!\n\n");
				printf("Data = %s\n\n", read_buf);
				//uncomment while giving demo to show process context in bottom half
				//close(fd);
				//fd = 0;
				//fd = open(file_name, O_RDWR);
				break;
			case '3':
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter Valid option\n");
				break;
		}
	}
	printf("Closing Driver\n");
	close(fd);
	return;

}

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		puts("Give command line args as follow :");
		puts("exicutable device_file_name operation");
		printf("operation option :\n 1) for simple read write : enter rw\n2) for ioctl : enter ioctl\n try again...\n");
		return 0;
	}
	printf("*********************************\n");
	printf("*******WWW.EmbeTronicX.com*******\n");
	printf("%s\n%s\n%s\n*********************\n", argv[0], argv[1], argv[2]);
	if(strcmp(argv[2],IOCTL_METHOD) == 0)
	{
		printf("calling ioctl_func\n");
		ioctl_func(argv[1]);
	}
	else if(strcmp(argv[2], READ_WRITE_CALL) == 0)
	{
		printf("calling rw_func\n");
		rw_func(argv[1]);
	}
	return 0;
}
