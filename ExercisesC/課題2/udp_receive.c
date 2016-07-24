#include <sys/types.h>     /* socket()
��Ȥ������ɬ��
*/
#include <sys/socket.h>    /*
Ʊ��
*/
#include <netinet/in.h>    /* INET
�ɥᥤ��Υ����åȤ�Ȥ������ɬ��
*/
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 10000
int main(void)
{
int sock;
struct sockaddr_in svr;
struct sockaddr_in sender;
int sender_len;
struct hostent *hp;
char buf[1024];
char senderstr[1024];
int len;
/*
�����åȤ�����
*/
if ( ( sock = socket(AF_INET,SOCK_DGRAM,0) ) < 0) {
perror("socket");
exit(1);
}
/* client
�����ѥ����åȤξ�������
*/
bzero(&svr,sizeof(svr));
svr.sin_family=AF_INET;
svr.sin_addr.s_addr=htonl(INADDR_ANY);
/*
�ۥ��Ȥ�
IP
���ɥ쥹�ϰ��̤�ʣ������ƥͥåȥ�����󥿥ե�����
(
�̿��μ������Ȥʤ�ϡ��ɥ��������뤤�ϲ��ۥϡ��ɥ�����
(
�롼�ץХå����󥿥ե������ʤ�
))
�˳�����Ƥ��Ƥ��롥
���饤����Ȥ��ɤ����饢���������뤫�ˤ�ä�
�����ФΤɤΥ��󥿥ե����������Τɤ줫��������Ƥ⤤���褦��
�磻��ɥ����ɤȤ���
INADDR_ANY
����ꤹ�롥
*/
svr.sin_port=htons(PORT);
/*
�����åȤ˥����åȥ��ɥ쥹�������Ƥ�
*/
if(bind(sock,(struct sockaddr*)&svr,sizeof(svr))<0) {
perror("bind");
exit(1);
}
/*
�ѥ��åȤμ�����ɽ��
*/
while(1){
//���������ѥ��åȤ������������(0.0.0.0��Ǥ�դΥ��ɥ쥹����Υѥ��åȤ����)
bzero(&sender,sizeof(sender));
sender_len = sizeof(sender);
if((len = recvfrom(sock, buf, sizeof(buf), 0,
(struct sockaddr*)&sender, (socklen_t*)&sender_len)) < 0){
perror("recvfrom");
exit(1);
}
inet_ntop(AF_INET, &sender.sin_addr, senderstr, sizeof(senderstr));
write(1, buf, len);
printf(" from [%s:%d]\n", senderstr, sender.sin_port);
}
}
