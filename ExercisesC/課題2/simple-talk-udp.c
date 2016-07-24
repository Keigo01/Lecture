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
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 10000
int main(int argc,char **argv)
{
  int sock;
 struct sockaddr_in svr;
 struct sockaddr_in sender;
 int sender_len;
 struct hostent *hp;
 char buf[1024];
 char senderstr[1024];
 int len;
 fd_set rfds;
 struct timeval tv;
 bzero(buf,sizeof(buf));
 
 /*
   �����åȤ�����
 */
 if ( ( sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP) ) < 0) {
  perror("socket");
 exit(1);
 }

 bzero(&svr,sizeof(svr));
 svr.sin_family=AF_INET;
 svr.sin_addr.s_addr=htonl(INADDR_ANY);
 
 svr.sin_port=htons(PORT);

  if(bind(sock,(struct sockaddr*)&svr,sizeof(svr))<0) {
   perror("bind");
   exit(1);
 }

 if ( ( hp = gethostbyname(argv[1]) ) == NULL ) {
   fprintf(stderr,"unknown host %s\n",argv[1]);
   exit(1);
 }
 bcopy(hp->h_addr,&svr.sin_addr,hp->h_length);

  do{
    /*
      ���Ϥ�ƻ뤹��ե����뵭�һҤν�����ѿ�
      rfds
      �˥��åȤ���
    */
    FD_ZERO(&rfds);     /* rfds
			   �������˽����
			*/
    FD_SET(0,&rfds);    /*
			  ɸ������
			*/
    FD_SET(sock,&rfds);  /*
			    ���饤����Ȥ�����դ��������å�
			  */

    /*
      �ƻ뤹���Ԥ����֤�
      1
      �ä�����
    */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    /*
      ɸ�����Ϥȥ����åȤ���μ�����Ʊ���˴ƻ뤹��
    */
    if(select(sock+1,&rfds,NULL,NULL,&tv)>0) {
      if(FD_ISSET(0,&rfds)) {  /*
				 ɸ�����Ϥ������Ϥ����ä��ʤ�
			       */
	/*
	  ɸ�����Ϥ����ɤ߹��ߥ��饤����Ȥ�����
	*/
	read(0,buf,sizeof(buf));
	sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&svr,sizeof(svr));
	bzero(buf,sizeof(buf));
      }
      if(FD_ISSET(sock,&rfds)) { /*
				    �����åȤ�����������ʤ�
				  */
	/*
	  �����åȤ����ɤ߹���ü���˽���
	*/
	bzero(&sender,sizeof(sender));
	sender_len = sizeof(sender);
	if((len = recvfrom(sock, buf, sizeof(buf), 0,
			   (struct sockaddr*)&sender, (socklen_t*)&sender_len)) < 0){
	  perror("recvfrom");
	  exit(1);
	}
	inet_ntop(AF_INET, &sender.sin_addr, senderstr, sizeof(senderstr));
	write(1, buf, len);
	bzero(buf,sizeof(buf));

      }
    }
  } while(1);

}
