#include	"unp.h"
#include	"myerr.h"

void sig_chld(int signo)
{
	pid_t 	pid;
	int	stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}
	
	return;
}

int main(int argc, char **argv)
{
	int 		listenfd, connfd, udpfd, maxfdp1, nready;
	char 		msg[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
	pid_t		childpid;
	const int 	on = 1;
	socklen_t 	len;
	fd_set		rset;
	ssize_t		n;
	
	/* Create TCP socket */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	/* set the address reuse */
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	
	/* Create UDP socket */
	udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);
	Bind(udpfd, (SA *) &servaddr, sizeof(servaddr));

	/* Init the select */
	Signal(SIGCHLD, sig_chld); /* must call waitpid */
	FD_ZERO(&rset);
	maxfdp1 = max(listenfd, udpfd) + 1;
	
	while(1) {
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		if((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
			if(errno == EINTR) {
				continue;
			} else {
				err_sys("Select error");
			}
		}

		if(FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &len);
			if((childpid = Fork()) == 0) {
				Close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			Close(connfd);
		}
		
		if(FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			n = Recvfrom(udpfd, msg, MAXLINE, 0, (SA *) &cliaddr, &len);
			Sendto(udpfd, msg, n, 0, (SA *) &cliaddr, len);
		}
	} 
}
 
