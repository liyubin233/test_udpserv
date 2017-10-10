#include	"unp.h"
#include	"myerr.h"
#include	"string.h"

#if 0
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE];
	struct sockaddr *preply_addr;
	socklen_t len;
	
	preply_addr = Malloc(servlen);

	while(Fgets(sendline, MAXLINE, fp) != NULL) {
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		len = servlen;
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
		if(len != servlen || memcmp(pservaddr, preply_addr, len) != 0) { /* len is wrong or add is wrong */
			printf("reply from %s ignored !\n", Sock_ntop(preply_addr, len));
			continue;
		}
		recvline[n] = 0; /* null terminated */
		Fputs(recvline, stdout);
	}
}
#else
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
        int n;
        char sendline[MAXLINE], recvline[MAXLINE];

	Connect(sockfd, (SA *) pservaddr, servlen);
        while(Fgets(sendline, MAXLINE, fp) != NULL) {
		Write(sockfd, sendline, strlen(sendline));
                n = Read(sockfd, recvline, MAXLINE);
                recvline[n] = 0; /* null terminated */
                Fputs(recvline, stdout);
        }
}
#endif

int main(int argc, char ** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	
	if(argc != 2) {
		err_quit("usage: udpcli <IPaddress>");
	}
		
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

	exit(0);
}
