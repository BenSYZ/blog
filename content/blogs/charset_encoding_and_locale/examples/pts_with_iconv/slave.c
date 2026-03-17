#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
int main(int argc, char *argv[], char *envp[]){
	if (argc != 2){
		printf("%s /dev/pts/<pts_num>\n", argv[0]);
		return 1;
	}
	char pts[64]="/dev/pts/";
	assert(strlen(argv[1]) < sizeof(pts)/sizeof(char)-1-strlen(pts));
	strcat(pts, argv[1]);


	struct termios tio;
	FILE *f_slave = fopen(pts, "r+");
	if (!f_slave){
		perror("f_slave: fopen");
		return 1;
	}
	tcgetattr(fileno(f_slave), &tio);
	tio.c_lflag &= ~ECHO;  // This stops the "mirror" effect
	tcsetattr(fileno(f_slave), TCSANOW, &tio);
	fclose(f_slave);

	//if (setsid() < 0)
	//{
	//	perror("setsid");
	//	return 1;
	//}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	freopen(pts, "r+", stdin);
	freopen(pts, "r+", stdout);
	freopen(pts, "r+", stderr);
	char *new_argv[]={[0]="/usr/bin/bash", [1]=NULL};
	execve(new_argv[0], new_argv , envp);


	return 0;
}

