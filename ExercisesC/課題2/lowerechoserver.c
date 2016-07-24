#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
int main(int argc,char **argv) {
int sock,csock;
struct sockaddr_in svr;
struct sockaddr_in clt;
struct hostent *cp;
int clen;
char rbuf[1024];
int nbytes;
int reuse;
/*
�����åȤ�����
*/
if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0) {
perror("socket");
exit(1);
}
/*
�����åȥ��ɥ쥹�����Ѥλ���
*/
reuse=1;
if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0) {
perror("setsockopt");
exit(1);
}
/* client
�����ѥ����åȤξ�������
*/
bzero(&svr,sizeof(svr));
svr.sin_family=AF_INET;
svr.sin_addr.s_addr=htonl(INADDR_ANY);
/*
����¦��
IP
���ɥ쥹��Ǥ��
*/
 svr.sin_port=htons(10120);  /*
�ݡ����ֹ�
10120
��𤷤Ƽ����դ���
*/
/*
�����åȤ˥����åȥ��ɥ쥹�������Ƥ�
*/
 if(bind(sock,(struct sockaddr *)&svr,sizeof(svr))<0) {
  perror("bind");
exit(1);
 }
/*
�Ԥ��������饤����ȿ�������
*/
 if (listen(sock,5)<0) {  /*
			   �Ԥ���������
5
�����
*/
  perror("listen");
 exit(1);
 }
do {
  /*
  ���饤����Ȥμ���
*/
clen = sizeof(clt);
 if ( ( csock = accept(sock,(struct sockaddr *)&clt,(socklen_t*)&clen) ) <0 ) {
  perror("accept");
exit(2);
 }
/*
  ���饤����ȤΥۥ��Ⱦ���μ���
*/
 cp = gethostbyaddr((char *)&clt.sin_addr,sizeof(struct in_addr),
		   AF_INET);
 printf("[%s]\n",cp->h_name);
 do {
  /*
  ���饤����Ȥ���Υ�å���������
*/
  if ( ( nbytes = read(csock,rbuf,sizeof(rbuf)) ) < 0) {
perror("read");
} else {
	for(int i = 0;i <= sizeof(rbuf); i++){
	 rbuf[i] = tolower(rbuf[i]);
	}
write(csock,rbuf,nbytes);
/*
����ʸ����򤽤Τޤޥ��饤����Ȥ��֤�
(echo) */
}
} while ( nbytes != 0 );
/* read()
��
0
���֤��ޤ�
(=End-Of-File)
�����֤�
*/
close(csock);
printf("closed\n");
} while(1);  /*
������³�׵�򷫤��֤������դ���
*/
}









