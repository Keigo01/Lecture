#include <sys/types.h>     /* socket()
を使うために必要
*/
#include <sys/socket.h>    /*
同上
*/
#include <netinet/in.h>    /* INET
ドメインのソケットを使うために必要
*/
#include <netdb.h>
/* gethostbyname()
を使うために必要
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
ソケットの生成
*/
if ( ( sock = socket(AF_INET,SOCK_DGRAM,0) ) < 0) {
perror("socket");
exit(1);
}
/* host(
ソケットの接続先
)
の情報設定
*/
bzero(&host,sizeof(host));
host.sin_family=AF_INET;
host.sin_port=htons(PORT);
inet_aton("192.168.16.255",&host.sin_addr);
setsockopt(sock,SOL_SOCKET,SO_BROADCAST, &yes,sizeof(yes));
/*
パケットの送信
*/
sendto(sock, argv[1], strlen(argv[1]), 0, (struct sockaddr*)&host,sizeof(host));
close(sock);
exit(0);
}
