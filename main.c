#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>

void ft_buff_to_file(void*);
void ft_file_to_buff(void*);

int main()
{
    char buff[100] = "";
    FILE *fd = fopen("file.txt", "w+");

}