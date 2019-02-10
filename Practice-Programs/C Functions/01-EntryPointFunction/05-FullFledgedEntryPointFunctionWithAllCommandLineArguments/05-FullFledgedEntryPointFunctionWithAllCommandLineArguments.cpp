#include<stdio.h>

int main(int argc,char *argv[],char *envp[]){
int i;

printf("\n\nHello World !!!\n\n");
printf("Number of Arguments passed to this program are : %d\n\n",argc);

printf("Command Line Arguments Passed To This Program Are : \n\n");

for(i=0;i<argc;i++){

	printf("Command Line Argument Number %d = %s\n", (i + 1),argv[i]);
	
}

printf("\n\n");

printf("First 20 Environmental Variables Passed To This Program Are : \n\n");

for(i=0;i<20;i++){

	printf("Environmental Variable Number %d = %s\n", (i + 1),envp[i]);
	
}

printf("\n\n");


return(0);
}