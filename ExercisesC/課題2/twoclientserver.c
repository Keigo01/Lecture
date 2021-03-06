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
  int csock,csock2,sock;
  struct sockaddr_in svr;
  struct sockaddr_in clt,clt2;
  struct hostent *cp,*cp2;
  int clen;
  int nbytes;
  int reuse;

  bzero(buf,sizeof(buf));
  bzero(tempname,sizeof(tempname));
  
  /*
    クライアントを受け付けたソケット
  */
  fd_set rfds;
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
  /*
    ソケットアドレス再利用の指定
  */
  reuse=1;
  if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))<0) {
    perror("setsockopt");
    exit(1);
  }
  /* client
     受付用ソケットの情報設定
  */
  bzero(&svr,sizeof(svr));
  svr.sin_family=AF_INET;
  svr.sin_addr.s_addr=htonl(INADDR_ANY);
  /*
    受付側の
    IP
    アドレスは任意
  */
  svr.sin_port=htons(10130);  /*
				ポート番号
				10130
				を介して受け付ける
			      */
  /*
    ソケットにソケットアドレスを割り当てる
  */
  if(bind(sock,(struct sockaddr *)&svr,sizeof(svr))<0) {
    perror("bind");
    exit(1);
  }
 

  /*
    クライアントからの接続待ち受けなど
  */
  /*
    待ち受けクライアント数の設定
  */
  if (listen(sock,5)<0) {  /*
			     待ち受け数に
			     5
			     を指定
			   */
    perror("listen");
    exit(1);
  }

  /*
    クライアントの受付
  */
  clen = sizeof(clt);
  if ( ( csock = accept(sock,(struct sockaddr *)&clt,(socklen_t*)&clen) ) <0 ) {
    perror("accept");
    exit(2);
  }

  if ( ( csock2 = accept(sock,(struct sockaddr *)&clt2,(socklen_t*)&clen) ) <0 ) {
    perror("accept");
    exit(2);
  }
  
  /*
    クライアントのホスト情報の取得
  */
  cp = gethostbyaddr((char *)&clt.sin_addr,sizeof(struct in_addr),
		     AF_INET);
  printf("client1 : [%s]\n",cp->h_name);

  cp2 = gethostbyaddr((char *)&clt2.sin_addr,sizeof(struct in_addr),
		     AF_INET);
  printf("client2 : [%s]\n",cp2->h_name);

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
    FD_SET(csock,&rfds);  
    FD_SET(csock2,&rfds);/* クライアントを受け付けたソケット
			  */

    /*
      監視する待ち時間を
      1
      秒に設定
    */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    /*
      標準入力とソケットからの受信を同時に監視する
    */
    if(select(csock+csock2+1,&rfds,NULL,NULL,&tv)>0) {
      if(FD_ISSET(csock,&rfds)) { /*
				    ソケットから受信したなら
				  */
	/*
	  ソケットから読み込み端末に出力
	*/
	read(csock,buf,sizeof(buf));
	write(csock2,buf,sizeof(buf));
	bzero(buf,sizeof(buf));
      }

       if(FD_ISSET(csock2,&rfds)) { /*
				    ソケット2から受信したなら
				  */
	/*
	  ソケットから読み込み端末に出力
	*/
	 read(csock2,buf,sizeof(buf));
	write(csock,buf,sizeof(buf));
	bzero(buf,sizeof(buf));
      }
    }

    
  } while(1);  /*
		 繰り返す
	       */
}
