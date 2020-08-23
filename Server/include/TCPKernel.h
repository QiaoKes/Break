#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H


#include "Thread_pool.h"
#include "TCPNet.h"
#include "packdef.h"
#include "Mysql.h"

typedef void (*PFUN)(int,char*);

typedef struct
{
	PackType m_type;
	PFUN m_pfun;
}ProtocolMap;

FILE *m_pFile;
int m_nFileSize;
int m_nPos;



int Open();
void Close();
void DealData(int,char*);

//注册
void RegisterRq(int,char*);
//登录
void LoginRq(int,char*);

//下載請求處理
void DownloadRq(int,char*);

void GetDownloadList( Queue*  plist,int userId);

//下載文件塊請求處理
void DownloadFileBlockRs(int,char*);

void UploadRq(int clientfd,char*szbuf);
void UploadFileBlockRq(int,char*);

void PressLikeRq(int clientfd,char*szbuf);

////读取上传的视频流信息
//void UpLoadVideoInfoRq(int,char*);
//void UpLoadVideoStreamRq(int,char*);
//void GetVideoRq(int,char*);
//char* GetVideoPath(char*);
//void QuitRq(int,char*);
//void PraiseVideoRq(int,char*);
//void GetAuthorInfoRq(int,char*);
#endif
