#include <stdio.h>
#define __USE_XOPEN_EXTENDED
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <physfs.h>
#include <termios.h>

#include <sys/select.h>
int print_select (int __nfds, fd_set *__restrict __readfds,
		   fd_set *__restrict __writefds,
		   fd_set *__restrict __exceptfds,
		   struct timeval *__restrict __timeout){
	int ret=0;
	ret = select(__nfds, __readfds, __writefds, __exceptfds, __timeout);
	//printf("select tv_sec=%ld, ret=%d\n", __timeout->tv_sec, ret);
	return ret;
}
int main(int argc, char *argv[]){
	FILE *f_ptmx = fopen("/dev/ptmx", "r+");
	if (!f_ptmx){
		perror("fopen");
		goto err;
	}

	if (grantpt(fileno(f_ptmx))){
		perror("grantpt");
		goto close_m_f;
	}

	if (unlockpt(fileno(f_ptmx))){
		perror("unlockpt");
		goto close_m_f;
	}


	char buf[512]={0};
	printf("%s, Ready\n", ptsname(fileno(f_ptmx)));


	struct timeval tv={
		.tv_sec=1,
		.tv_usec=0,
	};

	fd_set fd_set_r;
	FD_SET(fileno(f_ptmx), &fd_set_r);
	FD_SET(fileno(stdin), &fd_set_r);

	
	while (1){
		//while (fgets(buf, sizeof(buf)/sizeof(char), f_ptmx)) // get slave result
		//while (print_select(fileno(f_ptmx)+1, &fd_set_r, NULL, NULL, &tv)>0) // get slave result
		if (print_select(fileno(f_ptmx)+1, &fd_set_r, NULL, NULL, NULL)>0) // get slave result
		{
			//printf("in while\n");
			if (FD_ISSET(fileno(f_ptmx), &fd_set_r)){
				//printf("f_ptmx ready\n"); // TODO: after disable echo f_ptmx always ready
				memset(buf, 0, sizeof(buf)/sizeof(char));
				fgets(buf, sizeof(buf)/sizeof(char), f_ptmx);
				fwrite(buf, 1, strlen(buf), stdout);
				fflush(stdout);
			}else if(FD_ISSET(fileno(stdin), &fd_set_r)){
				//printf("stdin ready\n");
				memset(buf, 0, sizeof(buf)/sizeof(char));
				fgets(buf, sizeof(buf)/sizeof(char), stdin); // read from master
				//printf("debug: %s\n", buf);
				fwrite(buf, 1, strlen(buf), f_ptmx); // me master write to bash slave
				fflush(f_ptmx);
			}
		}
		FD_ZERO(&fd_set_r);
		FD_SET(fileno(f_ptmx), &fd_set_r);
		FD_SET(fileno(stdin), &fd_set_r);
		tv.tv_sec=1;
	}

	fclose(f_ptmx);
	return 0;

close_m_f:
	fclose(f_ptmx);
err:
	return 1;
}

	//while (1){
	//	memset(buf, 0, sizeof(buf)/sizeof(char));
	//	printf("Please write to dev\n");
	//	printf("getting...\n");
	//	sleep(1);
	//	fgets(buf, sizeof(buf)/sizeof(char), f_ptmx);
	//	printf("got...\n");
	//	sleep(1);
	//	printf("writing...\n");
	//	fwrite(buf, 1, strlen(buf), f_ptmx);
	//	fgets(buf, sizeof(buf)/sizeof(char), f_ptmx);
	//	printf("wroten\n");
	//	printf("Please read from dev\n");
	//	sleep(2);
	//}


