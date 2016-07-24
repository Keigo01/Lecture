#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 10120

int main(int argc,char **argv){
  int sock,csock;
  struct sockaddr_in svr;
  struct sockaddr_in clt;
  struct hostent *cp;
  int clen;
  char rbuf[1024];
  char buf[1024];
  int nbytes;
  int reuse;
   
/*
ソケットの生成
*/
  if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0) {
    perror("socket");
    exit(1);
  }

/* svr(
ソケットの接続先
)
の情報設定
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

  do{
    bzero(buf,sizeof(buf));
    
    if(read(0,buf,sizeof(buf))==0){
      break;
    }
    
  /*
    パケットの送信
  */
    write(sock,buf,sizeof(buf));
      
  
  /*
  サーバーからのメッセージ受信
  */
      if ( ( nbytes =read(sock,rbuf,sizeof(rbuf))) < 0) 
	perror("read");
      } else {
	
	write(1,rbuf,sizeof(rbuf));
      }
  } while(1);  /*
		 次の接続要求を繰り返し受け付ける
	       */
  close(sock);
  printf("closed\n");
  
}
