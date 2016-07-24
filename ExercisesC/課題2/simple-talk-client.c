#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define PORT 10130

int main(int argc,char **argv) {
  char buf[1024];
  char name[1024] = "[";
  char tempname[1024];
  struct sockaddr_in svr;
  struct sockaddr_in clt;
  struct hostent *cp;
  int clen;
  int nbytes;
  int reuse;
  
  int sock;
  fd_set rfds;

  bzero(buf,sizeof(buf));
  bzero(tempname,sizeof(tempname));
  
/* select()
   ���Ѥ���ե����뵭�һҽ���
*/
 struct timeval tv;   /* select()
			���֤äƤ���ޤǤ��Ԥ����֤���ꤹ���ѿ�
		     */
 
/*
  �����åȤ�����
*/
  if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0) {
    perror("socket");
    exit(1);
  }

  /* svr(
�����åȤ���³��
)
�ξ�������
*/
  bzero(&svr,sizeof(svr));
  svr.sin_family=AF_INET;
  svr.sin_port=htons(PORT);
  if ( ( cp = gethostbyname(argv[1]) ) == NULL ) {
    fprintf(stderr,"unknown server %s\n",argv[1]);
    exit(1);
  }
  bcopy(cp->h_addr,&svr.sin_addr,cp->h_length);
  
  connect(sock,&svr,sizeof(svr));

  printf("Please input your name!\n");
  scanf("%s",tempname);
  printf("Thanks you!\n");
  strcat(name,tempname);
  strcat(name,"]:");
  bzero(tempname,sizeof(tempname));
  strcpy(tempname,name);

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
      strcat(name,buf);
      write(sock,name,sizeof(name));
      bzero(buf,sizeof(buf));
      bzero(name,sizeof(name));
      strcpy(name,tempname);
    }
    if(FD_ISSET(sock,&rfds)) { /*
				  �����åȤ�����������ʤ�
				*/
      /*
	�����åȤ����ɤ߹���ü���˽���
      */
      read(sock,buf,sizeof(buf));
      write(0,buf,sizeof(buf));
      bzero(buf,sizeof(buf));
      
    }
  }
 } while(1);  /*
		�����֤�
	      */
}
