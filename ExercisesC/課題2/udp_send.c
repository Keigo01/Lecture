#include <sys/types.h>     /* socket()
��Ȥ������ɬ��
*/
#include <sys/socket.h>    /*
Ʊ��
*/
#include <netinet/in.h>    /* INET
�ɥᥤ��Υ����åȤ�Ȥ������ɬ��
*/
#include <netdb.h>
/* gethostbyname()
��Ȥ������ɬ��
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#define PORT 10000
int main(int argc,char **argv)
{
  int sock,ret;
struct sockaddr_in host;
struct hostent *hp;
int yes = 1;
/*
�����åȤ�����
*/
if ( ( sock = socket(AF_INET,SOCK_DGRAM,0) ) < 0) {
perror("socket");
exit(1);
}
/* host(
�����åȤ���³��
)
�ξ�������
*/
bzero(&host,sizeof(host));
host.sin_family=AF_INET;
host.sin_port=htons(PORT);
inet_aton("192.168.16.255",&host.sin_addr);
setsockopt(sock,SOL_SOCKET,SO_BROADCAST, &yes,sizeof(yes));
/*
�ѥ��åȤ�����
*/
sendto(sock, argv[1], strlen(argv[1]), 0, (struct sockaddr*)&host,sizeof(host));
close(sock);
exit(0);
}
