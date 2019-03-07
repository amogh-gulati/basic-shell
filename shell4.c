#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<signal.h>
#include<string.h>

/*void command_filename_overwrite(char* command[],char filename[]){
	int pid;
	
	if((pid=fork())==0){
		dup2(open(filename,O_RDWR|O_CREAT,0666),1);
		signal(SIGINT,SIG_DFL);		
		int status=execvp(command[0],command);
		if(status==-1){
			write(1,"Error in executing command \n",28);
		}
		exit(0);
	}
	else{
		wait(NULL);
	}	
}
void command_filename_append(char* command[],char filename[]){
	
	int pid;
	
	if((pid=fork())==0){
		dup2(open(filename,O_RDWR|O_APPEND|O_CREAT,0666),1);
		signal(SIGINT,SIG_DFL);		
		int status=execvp(command[0],command);
		if(status==-1){
			write(1,"Error in executing command \n",28);
		}
		exit(0);
	}
	else{
		wait(NULL);
	}
	
}*/
void redirectstdout(char filename[],int app){
	close(1);
	if(app==1)
		dup(open(filename,O_RDWR|O_APPEND|O_CREAT,0666));
	else if (app==0)
		dup(open(filename,O_RDWR|O_CREAT,0666));
}
void redirectstderr(char filename[],int app){
	close(2);
	if(app==1)
		dup(open(filename,O_RDWR|O_APPEND|O_CREAT,0666));
	else if (app==0)
		dup(open(filename,O_RDWR|O_APPEND|O_CREAT,0666));
}
void redirectstderr_stdout(){
	close(2);
	dup(1);
}
void redirectstdin(char filename[]){
	close(0);
	dup(open(filename,O_RDONLY));
}
/*void Pipe(char outcommand[], char incommand[]){
	int fd[2];
	pipe(fd);
	close(1);
	dup(fd[1]);
	close(0);
	dup(fd[0]);
	close(fd[1]);
	close(fd[0]);
	system(outcommand);
	system(incommand);
}*/
void Exit(){
	exit(0);
}	

void removemultiplespaces(char input[]){
	//CHeck this out geeksforgeeks.org/remove-extra-spaces-string  
	int n = strlen(input);
	char s[n];
	s[0]='\0';
	//remove the first spaces
	int i = 0;
	for(i;i<n && input[i]==' ';i++);
	//remove multiple spaces
	char prev = input[i];
	int j = 0;
	for(j;i<n;i++)
	{
		if(input[i]!=' ')
			s[j++]=input[i];
		else if (input[i]==' ' && prev!=' ')
			s[j++]=input[i];
		prev  = input[i];
	}
	n = strlen(s);
	//remove the ending spaces
	int e = n-1; 
	for(e;e>=0 && s[e]==' ';e--);
	s[e]='\0';
	//printf("%s\n",s);
	//printf("%d\n",n);
	strcpy(input,s);
}
void parseInput(char input[],char*list[]){
	int x=0;	
	int i=0;
	char *rest=input;
	char *token;
	
	while(token=strtok_r(rest,"|12",&rest)){
		//removemultiplespaces(token);
		list[x]=token;
		x++;
						
	}	
}

void runCommandsWrapper(char input[],int l,int n);


//function to execute the commands
int runCommands(char input[],int l,int n)
{
	int status = -1;
	signal(SIGINT,SIG_DFL);
	char *var[n];
	//j is for the arguments
	int j = 0;
	//k is counter for temp
	int k = 0;
	char temp[n];
	//to check the lowerbound for multiple pipes
	int i = 0;

	//string parsing 
	for(i=l;i<n;i++)
	{
		if(input[i]=='|')
		{
			break;
		}
		if(input[i]=='<')
		{
			int flag = 0;
			i++;
			if(input[i]=='<')
			{
				flag=1;
				i++;
			}	
			i++;
			char s[100];
			int j=0;
			for(i;i<n && input[i]!=' ';i++)
			{
				s[j] =input[i];
				j++; 
			}
			s[j] = '\0';
			redirectstdin(s);
			//check for the space problem
			if(i==n)
				break;
		}
		if(input[i]=='>')
		{
			//printf("is this entering thsi\n");
			int fds = 1;
			//printf("%s",input[i]);
			if(input[i-1]!=' ')
			{
				fds = input[i-1];
				fds = fds - 48;
				if(input[i-2]=='&')
				{
					fds = 3;
				}
			}
			int flag = 0;
			i++;
			if(input[i]=='>')
			{
				flag=1;
				i++;
			}	
			i++;
			char s[100];
			int j = 0;
			for(i;i<n && input[i]!=' ';i++)
			{
				s[j] = input[i];
				j++;
			}
			s[j] = '\0';
			//printf("%d\n",fds);
			//printf("just before redir\n");
			if(fds==1)
				{
					redirectstdout(s,flag);
				}
			if(fds==2)
				{
					redirectstderr(s,flag);
				}
			if(fds==3)
			{
				redirectstdout(s,flag);
				redirectstderr_stdout();
			}
			//printf("just after redir\n");
			k=0;
			if(i==n)
				break;
		}
		if(input[i]!=' ')
		{
			temp[k] = input[i];
			k++;
		}
		else if(input[i]==' ')
		{
			temp[k] = '\0';
			var[j] = (char*)malloc(sizeof(char)*k);
			strcpy(var[j],temp);
			j++;
			k=0;
		}
		//if we are processing the last character in input
		if(i==(n-1))
		{
			temp[k] = '\0';
			var[j] = (char*)malloc(sizeof(char)*k);
			strcpy(var[j],temp);
			j++;
			k=0;
			if(input[i]=='|')
				break;
		}
	}
	//end of string parsing and setting up var and redirections


	var[j]=NULL;
	printf("%d",j);
	/*for(int p=0;p<j;p++)
	{
		printf("%s\n",var[p]);
	}*/
	/*printf("these are the values\n");
	printf("%d\n",getpid());
	printf("%d\n",n);
	printf("%d\n",i);*/


	//if we have no pipe
	if(i==n)
	{
		//printf("if the command executed\n");
		status = execvp(var[0],var);
	}
	//if have have a pipe and commands to execute
	else if(i<n)
	{
		//printf("all hell broke loose\n");
		//this means we have used a pipe
		int pid;
		int fd[2];
		int ret = pipe(fd);
		if(ret==-1)
			return -1; //error in making pipe
		pid = fork();
		if(pid==0)
		{
			//this is the child process
			//this process would give input to parent
			//close the reading end
			close(fd[0]);
			//highjack the writing end
			close(1);
			dup(fd[1]);
			status = execvp(var[0],var);
			//command(var);
		}
		else if (pid>0)
		{
			//this is the parent process
			//this will accept the input from the child
			//close the writing end
			close(fd[1]);
			//highjack the reading end
			close(0);
			dup(fd[0]);
			runCommands(input,i+2,n);
			//its parent command has been executed 
			status  = 0;
		}
		else
		{
			printf("unable to fork\n");
		}
	}
	return status;
}


//function to fork
void runCommandsWrapper(char input[],int l,int n)
{
	int pidt;
	int status;
	if((pidt=fork())==0){
		status = runCommands(input,l,n);
	if(status==-1){
		write(STDOUT_FILENO,"Error in executing command \n",28);
		}
		exit(0);
	}
	else
		wait(NULL);
	//printf("  lkjsdflkslkfslkfsfkdlslkf   ;sknflksdfmsklf   %d\n",getpid());
}


int main(){
	//to stop the sigint from closing the shell itself
	signal(SIGINT,SIG_IGN);
	while(1){	
			
		int t[2];//Saving initial values of fds which Im loading at end.
		pipe(t);
		dup2(0,t[0]);
		dup2(1,t[1]);
		int u[2];
		pipe(u);
		dup2(2,u[0]);



		//command prompt
		printf("XD ");
		//input string
		char input[512];
		//scanf("%s",input);
		//changing the delimiter to newline only
		scanf ("%[^\n]%*c", input);
		int n = strlen(input);
		//printf("command lenght %d",n);
		//shell functions		
		if(strncmp(input,"exit",4)==0)
			exit(0);
		if(strncmp(input,"cd",2)==0)
		{
			char s[100]; 
    		// printing current working directory 
			for(int i=3;i<n;i++)
			{
				s[i-3] = input[i];
			}
			//changing the directory
			chdir(s);
		}


		//running executables after creating a new process
		runCommandsWrapper(input,0,n);

		//resetting the fds
		dup2(t[0],0);
		dup2(t[1],1);
		dup2(u[0],2);
		close(t[0]);
		close(t[1]);
		close(u[0]);
		close(u[1]);	
	}	
	return 0;
}
		
	

