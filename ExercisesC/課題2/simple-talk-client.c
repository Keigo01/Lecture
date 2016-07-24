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
   で用いるファイル記述子集合
*/
 struct timeval tv;   /* select()
			が返ってくるまでの待ち時間を指定する変数
		     */
 
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

  printf("Please input your name!\n");
  scanf("%s",tempname);
  printf("Thanks you!\n");
  strcat(name,tempname);
  strcat(name,"]:");
  bzero(tempname,sizeof(tempname));
  strcpy(tempname,name);

do{
  /*
    入力を監視するファイル記述子の集合を変数
    rfds
    にセットする
  */
  FD_ZERO(&rfds);     /* rfds
			 を空集合に初期化
		      */
  FD_SET(0,&rfds);    /*
			標準入力
		      */
  FD_SET(sock,&rfds);  /*
クライアントを受け付けたソケット
			*/
  tv.tv_sec = 1;
tv.tv_usec = 0;
  
  /*
    標準入力とソケットからの受信を同時に監視する
  */
  if(select(sock+1,&rfds,NULL,NULL,&tv)>0) {
    if(FD_ISSET(0,&rfds)) {  /*
			       標準入力から入力があったなら
			     */
      /*
	標準入力から読み込みクライアントに送信
      */
      read(0,buf,sizeof(buf));
      strcat(name,buf);
      write(sock,name,sizeof(name));
      bzero(buf,sizeof(buf));
      bzero(name,sizeof(name));
      strcpy(name,tempname);
    }
    if(FD_ISSET(sock,&rfds)) { /*
				  ソケットから受信したなら
				*/
      /*
	ソケットから読み込み端末に出力
      */
      read(sock,buf,sizeof(buf));
      write(0,buf,sizeof(buf));
      bzero(buf,sizeof(buf));
      
    }
  }
 } while(1);  /*
		繰り返す
	      */
}
