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

int main(int argc,char **argv) {
  char buf[1024];
  char tempname[1024];
  char name[1024] = "[";
  int csock,sock;
  struct sockaddr_in svr;
  struct sockaddr_in clt;
  struct hostent *cp;
  int clen;
  int nbytes;
  int reuse;

  bzero(buf,sizeof(buf));
  bzero(tempname,sizeof(tempname));
  
  /*
    ���饤����Ȥ�����դ��������å�
  */
  fd_set rfds;
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
  svr.sin_port=htons(10130);  /*
				�ݡ����ֹ�
				10130
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
    ���饤����Ȥ������³�Ԥ������ʤ�
  */
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

  printf("Please input your name!\n");
  scanf("%s",tempname);
  printf("Thanks you!\n");
  strcat(name,tempname);
  strcat(name,"]:");
  bzero(tempname,sizeof(tempname));
  strcpy(tempname,name);

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
    FD_SET(csock,&rfds);  /*
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
    if(select(csock+1,&rfds,NULL,NULL,&tv)>0) {
      if(FD_ISSET(0,&rfds)) {  /*
				 ɸ�����Ϥ������Ϥ����ä��ʤ�
			       */
	/*
	  ɸ�����Ϥ����ɤ߹��ߥ��饤����Ȥ�����
	*/
	read(0,buf,sizeof(buf));
	strcat(name,buf);
	write(csock,name,sizeof(name));
	bzero(buf,sizeof(buf));
	bzero(name,sizeof(name));
	strcpy(name,tempname);
      }
      if(FD_ISSET(csock,&rfds)) { /*
				    �����åȤ�����������ʤ�
				  */
	/*
	  �����åȤ����ɤ߹���ü���˽���
	*/
	read(csock,buf,sizeof(buf));
	write(0,buf,sizeof(buf));
	bzero(buf,sizeof(buf));
      }
    }
  } while(1);  /*
		 �����֤�
	       */
}
