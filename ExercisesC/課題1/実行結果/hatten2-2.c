#include <stdio.h>
#include <string.h>
#define COUNT 100
int main(int ac, char* av[]){
int i;

//char message[] = "hello";
char message[] = "hello\n";
for(i = 0; i < COUNT; i ++){

fwrite(message, strlen(message), 1, stdout);
//write(0, message, strlen(message));
}
return 0;
}
