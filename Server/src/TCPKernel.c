#include<TCPKernel.h>
#include "packdef.h"

Queue* FileQueue = 0;

static const ProtocolMap m_ProtocolMapEntries[] =
{
//	{_DEF_PROTOCOL_REGISTER_RQ,RegisterRq},
//	{_DEF_PROTOCOL_LOGIN_RQ,LoginRq},
//	{_DEF_PROTOCOL_UPLOADVIDEOHEAD_RQ,UpLoadVideoInfoRq},
//	{_DEF_PROTOCOL_UPLOADVIDEOSTREAM_RQ,UpLoadVideoStreamRq},
//	{_DEF_PROTOCOL_GETAUTHORVIDEOSTREAMHEAD_RQ,GetVideoRq},
//	{_DEF_PROTOCOL_QUITAUTHOR_RQ,QuitRq},
//	{_DEF_PROTOCOL_GIVENIDEOEVALUATE_RQ,PraiseVideoRq},
//	{_DEF_PROTOCOL_GETAUTHORINFO_RQ,GetAuthorInfoRq},
//	{0,0}
    {DEF_PACK_LOGIN_RQ,LoginRq},
    {DEF_PACK_REGISTER_RQ,RegisterRq},
    {DEF_PACK_DOWNLOAD_RQ,DownloadRq},
    {DEF_PACK_DOWNLOAD_FILEBLOCK_RS,DownloadFileBlockRs},
    {DEF_PACK_UPLOAD_RQ,UploadRq},
    {DEF_PACK_UPLOAD_FILEBLOCK_RQ,UploadFileBlockRq},
    {DEF_PACK_PRESSLIKE_RQ, PressLikeRq},
	{0,0}
};
#define RootPath   "/home/colin/Video/"

int Open()
{
	if(ConnectMysql("localhost","root","colin123","VideoServer") == FALSE)
	{
		printf("Conncet Mysql Failed...\n");
		return FALSE;
	}
	else
	{
		printf("MySql Connect Success...\n");
	}
	if(InitNetWork() == FALSE)
	{
		printf("InitNetWork Failed...\n");
		return FALSE;
	}
	else
	{
		printf("Init Net Success...\n");
	}
	m_pFile = NULL;
	m_nFileSize = 0;
	m_nPos = 0;

	q_Init(&FileQueue);
	return TRUE;
}

void Close()
{
	DisConnect();
	UnInitNetWork();
}


void DealData(int clientfd,char *szbuf)
{
	PackType *pType = (PackType*)szbuf;
	int i = 0;
	while(1)
	{
		if(*pType == m_ProtocolMapEntries[i].m_type)
		{
			(*m_ProtocolMapEntries[i].m_pfun)(clientfd,szbuf);
		}
		else if(m_ProtocolMapEntries[i].m_type == 0 &&
				m_ProtocolMapEntries[i].m_pfun == 0)
			return;
		++i;
	}
	return;
}

//
//void GetAuthorInfoRq(int clientfd,char *szbuf)
//{
//	printf("clientfd:%d GetAuthorInfoRq\n",clientfd);
//	STRU_GETAUTHORINFO_RQ *sgr = (STRU_GETAUTHORINFO_RQ*)szbuf;
//	char m_username[_DEF_SIZE];
//	memset(m_username,0,_DEF_SIZE);
//	strcpy(m_username,sgr->m_username);
//	Queue* pQueue = NULL;
//	q_Init(&pQueue);
//	char szsql[_DEF_SQLIEN];
//	memset(szsql,0,_DEF_SQLIEN);
//	sprintf(szsql,"select count(*),sum(m_like) from VideoInfo where m_authorname = '%s'",m_username);
//	printf("%s\n",szsql);
//	if(SelectMysql(szsql,2,pQueue) == FALSE)
//	{
//		err_str("SelectMySql Falied:",-1);
//	}
//	int nCount = 0;
//	nCount = atoi((char*)q_Pop(pQueue));
//	int nPraise = 0;
//	nPraise = atoi((char*)q_Pop(pQueue));
//	memset(szsql,0,sizeof(szsql));
//	sprintf(szsql,"select m_videoname from VideoInfo where m_authorname = '%s'",m_username);
//	printf("nCount:%d\nnPrasise:%d\n%s\n",nCount,nPraise,szsql);
//	if(SelectMysql(szsql,1,pQueue) == FALSE)
//	{
//		err_str("SelectMySql Falied:",-1);
//	}
//	STRU_GETAUTHORINFO_RS sgrs;
//	sgrs.m_nType = _DEF_PROTOCOL_GETAUTHORINFO_RS;
//	sgrs.nNum_praise = nPraise;
//	strcpy(sgrs.m_username,m_username);
//	for(int i=0;i<nCount;i++)
//	{
//		strcat(sgrs.szbuf,(char*)q_Pop(pQueue));
//		strcat(sgrs.szbuf,"\n");
//	}
//	strcat(sgrs.szbuf,"\0");
//	printf("%s",sgrs.szbuf);
//
//	SendData(clientfd,(char*)&sgrs,sizeof(sgrs));
//
//
//}
//
//void PraiseVideoRq(int clientfd,char *szbuf)
//{
//	printf("clientfd:%d PraiseVideoRq\n",clientfd);
//	STRU_GIVENIDEOEVALUATE_RQ *psgr = (STRU_GIVENIDEOEVALUATE_RQ*)szbuf;
//	char author_Name[_DEF_SIZE];
//	char video_Name[_DEF_SIZE];
//	char szsql[_DEF_SQLIEN];
//	memset(author_Name,0,_DEF_SIZE);
//	memset(video_Name,0,_DEF_SIZE);
//	memset(szsql,0,_DEF_SIZE);
//	strcpy(author_Name,psgr->su_videoinfo.m_username);
//	strcpy(video_Name,psgr->su_videoinfo.m_videoname);
//	int praise = psgr->su_videoinfo.nNum_praise;
//	sprintf(szsql,"update VideoInfo set m_like=%d where m_videoname='%s'",praise,video_Name);
//	printf("%s\n",szsql);
//	STRU_GIVENIDEOEVALUATE_RS sgr;
//	if(UpdataMysql(szsql) == FALSE)
//	{
//		printf("Updata MySql Failed...\n");
//		sgr.m_lResult = _GIVENIDEOEVALUATE_fail;
//	}
//	else
//	{
//		sgr.m_nType = _DEF_PROTOCOL_GIVENIDEOEVALUATE_RS;
//		sgr.m_lResult = _GIVENIDEOEVALUATE_success;
//	}
//	memset(szsql,0,_DEF_SQLIEN);
//	sprintf(szsql,"update UserFeature set ntype_%d = ntype_%d +1 where m_authorname = '%s'",psgr->su_videoinfo.videotype,psgr->su_videoinfo.videotype,author_Name);
//	printf("%s\n",szsql);
//	if(UpdataMysql(szsql) == FALSE)
//	{
//		printf("Updata MySql Failed...\n");
//	}
//	SendData(clientfd,(char*)&sgr,sizeof(sgr));
//}
//void QuitRq(int clientfd,char* szbuf)
//{
//	printf("clientfd:%d QuitRq\n",clientfd);
//	close(clientfd);
//
//}
//char* GetVideoPath(char* szbuf)
//{
//	char *rel_path = (char*)malloc(sizeof(char)*_DEF_SIZE);
//	memset(rel_path,0,_DEF_SIZE);
//	char szsql[_DEF_SQLIEN];
//	STRU_GETAUTHORVIDEOSTREAMHEAD_RQ *psgr = (STRU_GETAUTHORVIDEOSTREAMHEAD_RQ*)szbuf;
//	Queue* pQueue = NULL;
//	q_Init(&pQueue);
//	sprintf(szsql,"select ntype_1,ntype_2,ntype_3,ntype_4,ntype_5 from UserFeature where m_authorname = '%s'",psgr->m_username);
//	int arrtype[5] = {0};
//	float type[5] = {0.0};
//	char path[5][_DEF_SIZE] = {0};
//	memset(path,0,sizeof(char)*5*_DEF_SIZE);
//	printf("%s\n",szsql);
//	if(SelectMysql(szsql,5,pQueue) == FALSE)
//	{
//		err_str("SelectMySql Falied:",-1);
//	}
//	memset(szsql,0,sizeof(szsql));
//	if(q_GetNum(pQueue) == 0)
//	{
//		return 0;
//	}
//	int i;
//	int sum = 0;
//	for(i=0;i<5;i++)
//	{
//		arrtype[i] = atoi((char*)q_Pop(pQueue));
//		sum += arrtype[i];
//	}
//	if(sum == 0)
//	{
//		memset(szsql,0,sizeof(szsql));
//
//		//sprintf(szsql,"select m_path from VideoInfo where (m_like = (select max(m_like) from VideoInfo))");
//
//		sprintf(szsql,"select a.m_path from (select VideoInfo.m_path,VideoInfo.m_like from VideoInfo left join %s on VideoInfo.m_path = %s.m_path where %s.m_path is null)a where (a.m_like = (select max(b.m_like) from (select VideoInfo.m_like from VideoInfo left join %s on VideoInfo.m_path = %s.m_path where %s.m_path is null)b ) )",psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username);
//		printf("%s\n",szsql);
//		if(SelectMysql(szsql,1,pQueue) == FALSE)
//		{
//			err_str("SelectMySql Falied:",-1);
//		}
//		if(q_GetNum(pQueue) == 0)
//		{
//			q_Destroy(&pQueue);
//			return 0;
//		}
//		strcpy(rel_path,(char*)q_Pop(pQueue));
//		printf("%s\n",rel_path);
//	}
//	else
//	{
//		int nTemp = 0;
//		int n_typebj = 5;
//		for(i=0;i<5;i++)
//		{
//			type[i] = arrtype[i]/(float)sum;
//			printf("type[%d]:%f\n",i,type[i]);
//			memset(szsql,0,sizeof(szsql));
//			sprintf(szsql,"select a.m_path,a.m_like from (select VideoInfo.m_path,VideoInfo.m_like from VideoInfo left join %s on VideoInfo.m_path = %s.m_path where %s.m_path is null)a where (a.m_like = (select max(b.m_like) from (select VideoInfo.m_like from VideoInfo left join %s on VideoInfo.m_path = %s.m_path where %s.m_path is null and (VideoInfo.m_type = %d))b))",psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username,psgr->m_username,i+1);
//			printf("%s\n",szsql);
//			if(SelectMysql(szsql,2,pQueue) == FALSE)
//			{
//				err_str("SelectMySql Falied:",-1);
//			}
//			if(q_GetNum(pQueue) == 0)
//			{
//				continue;
//			}
//			strcpy(path[i],(char*)q_Pop(pQueue));
//			//type[i] = type[i]*(atoi((char*)q_Pop(pQueue)));
//			type[i] = (arrtype[i]*(1.0)/sum)*(atoi((char*)q_Pop(pQueue)));
//			if(type[i] >= nTemp)
//			{
//				nTemp = type[i];
//				n_typebj = i;
//			}
//		}
//		if(5 == n_typebj)
//		{
//			q_Destroy(&pQueue);
//			return 0;
//		}
//		else
//			strcpy(rel_path,path[n_typebj]);
//	}
//	q_Destroy(&pQueue);
//	memset(szsql,0,sizeof(szsql));
//	sprintf(szsql,"insert into %s(m_path) values('%s')",psgr->m_username,rel_path);
//	printf("%s\n",szsql);
//	if(UpdataMysql(szsql) == FALSE)
//	{
//		err_str("Updata MySql Failed...",-1);
//	}
//
//	return rel_path;
//
//}
//
//
//void GetVideoRq(int clientfd,char* szbuf)
//{
//	//TODO:path NOW 不变的path
//	char *szpath = GetVideoPath(szbuf);
//	if(szpath == NULL) return;
//	int pos = 0;
//	while( szpath[pos] != '\0')pos++;
//	while( szpath[pos-1] != '/')pos--;
//	char szName[_DEF_SIZE];
//	memset(szName,0,_DEF_SIZE);
//	int namepos = 0;
//	for(;szpath[pos] != '\0';pos++,namepos++)szName[namepos] = szpath[pos];
//	szName[namepos] = '\0';
//	printf("path:%s name:%s\n",szpath,szName);
//	FILE* pFile = NULL;
//	pFile = fopen(szpath,"rb");
//	if(pFile == NULL)return;
//	fseek(pFile,0,SEEK_END);
//	long long nFileSize = ftell(pFile);
//	fseek(pFile,0,SEEK_SET);
//	STRU_GETAUTHORVIDEOSTREAMHEAD_RS sgr;
//	strcpy(sgr.su_videoinfo.m_videoname,szName);
//	sgr.m_nType = _DEF_PROTOCOL_GETAUTHORVIDEOSTREAMHEAD_RS;
//	sgr.n_len = nFileSize;
//	Queue* pQueue = NULL;
//	q_Init(&pQueue);
//	char szsql[_DEF_SQLIEN];
//	sprintf(szsql,"select m_authorname,m_like,m_type from VideoInfo where m_videoname = '%s'",szName);
//	printf("%s\n",szsql);
//	if(SelectMysql(szsql,3,pQueue) == FALSE)
//		err_str("SelectMySql Falied:",-1);
//	if(q_GetNum(pQueue) == 0)
//		return;
//	else
//	{
//		strcpy(sgr.su_videoinfo.m_username,(char*)q_Pop(pQueue));
//		sgr.su_videoinfo.nNum_praise = atoi((char*)q_Pop(pQueue));
//		sgr.su_videoinfo.videotype = atoi((char*)q_Pop(pQueue));
//		q_Destroy(&pQueue);
//	}
//	SendData(clientfd,(char*)&sgr,sizeof(sgr));
//	while(1)
//	{
//		STRU_GETAUTHORVIDEOSTREAM_RS sr;
//		sr.m_nType = _DEF_PROTOCOL_GETAUTHORVIDEOSTREAM_RS;
//		memset(sr.szbuf,0,sizeof(sr.szbuf));
//		sr.n_len = fread(sr.szbuf,sizeof(char),sizeof(sr.szbuf),pFile);
//		if(sr.n_len == 0)
//			break;
//		SendData(clientfd,(char*)&sr,sizeof(sr));
//	}
//	free(szpath);
//	szpath = NULL;
//}
//void UpLoadVideoInfoRq(int clientfd,char* szbuf)
//{
//
//	printf("clientfd:%d UpLoadVideoInfoRq\n",clientfd);
//	STRU_UPLOADVIDEOHEAD_RQ *psur = (STRU_UPLOADVIDEOHEAD_RQ*)szbuf;
//	char szpath[_DEF_SIZE];
//	sprintf(szpath,"..//view//%s//%s",psur->su_videoinfo.m_username,psur->su_videoinfo.m_videoname);
//	char szdir[_DEF_SIZE];
//	sprintf(szdir,"..//view//%s",psur->su_videoinfo.m_username);
//	printf("%s\n",szdir);
//	mkdir(szdir,0755);
//	m_pFile = fopen(szpath,"wb");
//	char szsql[_DEF_SQLIEN];
//	sprintf(szsql,"insert into VideoInfo (m_authorname,m_path,m_videoname,m_like,m_type) values('%s','%s','%s',%d,%d)",psur->su_videoinfo.m_username,szpath,psur->su_videoinfo.m_videoname,psur->su_videoinfo.nNum_praise,psur->su_videoinfo.videotype);
//	printf("%s\n",szsql);
//	if(UpdataMysql(szsql) == FALSE)
//		printf("Updata MySql Failed...\n");
//	if(m_pFile)
//	{
//		m_nFileSize = psur->n_len;
//		m_nPos = 0;
//	}
//}
//void UpLoadVideoStreamRq(int clientfd,char* pszbuf)
//{
//	printf("clientfd:%d UpLoadVideoStreamRq\n",clientfd);
//	STRU_UPLOADVIDEOSTREAM_RS sur;
//	sur.m_nType = _DEF_PROTOCOL_UPLOADVIDEOSTREAM_RS;
//	int bj = 1;
//	printf("sizeof pszbuf :%d\n",sizeof(pszbuf));
//	char *szbuf = 0;
//	STRU_UPLOADVIDEOSTREAM_RQ *psur_0 = (STRU_UPLOADVIDEOSTREAM_RQ*)pszbuf;
//	int nlen = fwrite(psur_0->szbuf,sizeof(char),psur_0->n_len,m_pFile);
//	while(bj)
//	{
//		if(psur_0 == NULL)
//		{
//			STRU_UPLOADVIDEOSTREAM_RQ *psur = (STRU_UPLOADVIDEOSTREAM_RQ*)szbuf;
//			if(!m_pFile)
//			{
//				return;
//			}
//			nlen = fwrite(psur->szbuf,sizeof(char),psur->n_len,m_pFile);
//			free(szbuf);
//			szbuf = NULL;
//		}
//		if(nlen>0)
//			m_nPos += nlen;
//		if(m_nPos == m_nFileSize)
//		{
//			fclose(m_pFile);
//			m_pFile = NULL;
//			m_nPos = 0;
//			m_nFileSize = 0;
//			SendData(clientfd,(char*)&sur,sizeof(sur));
//			printf("----------------\n");
//			bj = 0;
//		}
//		else
//		{
//			int nPackSize = 0;
//			int nRelReadNum = recv(clientfd,&nPackSize,sizeof(nPackSize),0);
//			szbuf = (char*)malloc(sizeof(char)*nPackSize);
//			int nOffSet = 0;
//			nRelReadNum = 0;
//			while(nPackSize)
//			{
//				nRelReadNum = recv(clientfd,szbuf+nOffSet,nPackSize,0);
//				if(nRelReadNum > 0)
//				{
//					nOffSet += nRelReadNum;
//					nPackSize -= nRelReadNum;
//				}
//			}
//		}
//		nlen = 0;
//		psur_0 = NULL;
//	}
//	printf("szbuf=%p bj = %d\n",szbuf,bj);
//}
void LoginRq(int clientfd,char* szbuf)
{
	printf("clientfd:%d LoginRq\n",clientfd);
	STRU_LOGIN_RQ *rq = (STRU_LOGIN_RQ*)szbuf;
	STRU_LOGIN_RS rs;
    rs.m_nType = DEF_PACK_LOGIN_RS;

	char szsql[_DEF_SQLIEN];
	bzero(szsql,sizeof(szsql));
	Queue* pQueue = NULL;
	q_Init(&pQueue);
	sprintf(szsql,"select password ,id from t_UserData where name = '%s'",rq->m_szUser);
	printf("%s\n",szsql);
	if(SelectMysql(szsql,2,pQueue) == FALSE)
	{
		err_str("SelectMySql Falied:",-1);
		return;
	}
	if(q_GetNum(pQueue) == 0)
	{
		rs.m_lResult = userid_no_exist;
	}
	else
	{
		char *strpassword = (char*)q_Pop(pQueue);
		rs.m_lResult = password_error;
		if(0 == strcmp(rq->m_szPassword,strpassword))
		{
			rs.m_lResult = login_sucess;
			char * id = (char *)q_Pop(pQueue);
			rs.m_UserID = atoi(id);
			printf("UserID:%d\n",rs.m_UserID);
		}
	}
	SendData(clientfd,(char*)&rs,sizeof(rs));
}
//
void RegisterRq(int clientfd,char* szbuf)
{
	printf("clientfd:%d RegisterRq\n",clientfd);
	STRU_REGISTER_RQ *rq = (STRU_REGISTER_RQ*)szbuf;
	STRU_REGISTER_RS rs;

    rs.m_nType = DEF_PACK_REGISTER_RS;
	char szsql[_DEF_SQLIEN];
	bzero(szsql,sizeof(szsql));
	Queue* pQueue = NULL;
	q_Init(&pQueue);
	//判断角色是否存在
	sprintf(szsql,"select  name from t_UserData where name = '%s'",rq->m_szUser);
	printf("%s\n",szsql);
	if(SelectMysql(szsql,1,pQueue) == FALSE)
	{
		err_str("SelectMySql Falied:",-1);
		return;
	}
	if(q_GetNum(pQueue) > 0)
	{
		rs.m_lResult = userid_is_exist;
	}
	else
	{
		rs.m_lResult = register_sucess;
		sprintf(szsql,"insert into t_UserData (name,password,food  , funny, ennegy ,dance , music ,  video ,  outside, edu) values('%s','%s',%d,%d,%d,%d,%d,%d,%d,%d)",rq->m_szUser,rq->m_szPassword
                                        ,rq->food  , rq->funny, rq->ennegy ,rq->dance , rq->music ,  rq->video ,  rq->outside, rq->edu);
		printf("%s\n",szsql);
		if(UpdataMysql(szsql) == FALSE)
		{
			err_str("Updata MySql Failed...",-1);
		}

		//生成一个属于他的路径
		char path[MAX_PATH] = {0};
        sprintf( path ,"%sflv/%s/" , RootPath,rq->m_szUser );
		mkdir(path, S_IRWXU );
	}
	q_Destroy(&pQueue);
	SendData(clientfd,(char*)&rs,sizeof(rs));
}


void DownloadRq(int clientfd,char*szbuf)
{
    printf("clientfd:%d DownloadRq\n",clientfd);
    STRU_DOWNLOAD_RQ *rq = (STRU_DOWNLOAD_RQ*)szbuf;

    printf("id:%d\n",rq->m_nUserId);
    Queue* pList = NULL;
	q_Init(&pList);

    GetDownloadList(  pList,  rq->m_nUserId);

    while(q_GetNum(pList) != 0)
    {
        FileInfo * pInfo = (FileInfo*)q_Pop(pList);
        STRU_DOWNLOAD_RS rs;
        rs.m_nFileId = pInfo->m_nFileID;
        rs.m_nFileSize = pInfo->m_nFileSize;
        rs.m_nType = DEF_PACK_DOWNLOAD_RS;
        rs.m_nVideoId = pInfo->m_VideoID;
        strcpy( rs.m_rtmp ,  pInfo->m_szRtmp);
        strcpy( rs.m_szFileName , pInfo->m_szFileName);
        SendData(clientfd,(char*)&rs,sizeof(rs));

        pInfo->pFile = fopen(pInfo->m_szFilePath , "r");
        printf("%s\n",pInfo->m_szFilePath);
        if( pInfo->pFile )
        {
            while(1)
            {
                //文件内容 内容长度 文件Id,文件位置--传输请求
                STRU_DOWNLOAD_FILEBLOCK_RQ blockrq;
                blockrq.m_nType = DEF_PACK_DOWNLOAD_FILEBLOCK_RQ;
                int64_t nRelReadNum = (int64_t)fread(blockrq.m_szFileContent,1,MAX_CONTENT_LEN,pInfo->pFile);

                blockrq.m_nBlockLen = nRelReadNum;
                blockrq.m_nFileId = pInfo->m_nFileID;
                blockrq.m_nUserId = pInfo->m_nUserId;

                printf("nRelReadNum:%d\n",blockrq.m_nBlockLen);
                SendData(clientfd,(char*)&blockrq,sizeof(blockrq));

                pInfo->m_nPos += nRelReadNum;

                if(pInfo->m_nPos == pInfo->m_nFileSize)
                {
                    fclose(pInfo->pFile);
                    free( pInfo );
                    break;
                }
            }
        }
    }
    q_Destroy(&pList);

}

void UploadRq(int clientfd,char*szbuf)
{
    printf("clientfd:%d UploadRq\n",clientfd);
    STRU_UPLOAD_RQ *rq = (STRU_UPLOAD_RQ *)szbuf;
    FileInfo *info = (FileInfo*)malloc(sizeof(FileInfo));

    info->m_nFileID = rq->m_nFileId ;
    info->m_nPos = 0;
    info->m_nFileSize = rq->m_nFileSize;
    memcpy( info->m_szFileName , rq->m_szFileName , MAX_PATH);
    memcpy( info->m_szFileType , rq->m_szFileType , MAX_PATH);
    info->m_nUserId = rq->m_UserId;
    memcpy(  info->m_Hobby , rq->m_szHobby , DEF_HOBBY_COUNT);
    //获取用户名字
    char szsql[_DEF_SQLIEN];
	bzero(szsql,sizeof(szsql));
	Queue* pQueue = NULL;
	q_Init(&pQueue);
	//判断角色是否存在
	sprintf(szsql,"select name from t_UserData where id = %d;",rq->m_UserId);
	printf("%s\n",szsql);
	if(SelectMysql(szsql,1,pQueue) == FALSE)
	{
		err_str("SelectMySql Falied:",-1);
		free( info );
		q_Destroy(&pQueue);
		return;
	}
    else{
		char * szUserName = (char*)q_Pop(pQueue);
		sprintf( info->m_UserName ,"%s",szUserName);
		sprintf( info->m_szFilePath ,"%sflv/%s/%s",RootPath,szUserName,rq->m_szFileName);
    }
    info->m_VideoID = 0;
    info->pFile = fopen(info->m_szFilePath , "w");
    if( info->pFile )
    {
        q_Push( FileQueue , (void*)info);
    }else{
        free( info );
    }
    q_Destroy(&pQueue);
}
char *  GetPicNameOfVideo( char * videoName)
{

    char *  picName = (char * )malloc( MAX_PATH ) ;
    memset( picName , 0 , MAX_PATH );
    int i;
    int nLen = strlen(videoName);
    for( i = nLen - 1 ; i>= 0 ; i--)
    {
        if( videoName[i] == '.')
         break;
    }
    memcpy(picName , videoName , i+1);
    strcat(picName , "jpg");
    return picName;
}

void UploadFileBlockRq(int clientfd,char*szbuf)
{
    STRU_UPLOAD_FILEBLOCK_RQ *rq = (STRU_UPLOAD_FILEBLOCK_RQ *)szbuf;

        FileInfo * info = 0;
        int64_t nLen = 0 ;

        Myqueue * tmp = FileQueue->pHead;
        while( tmp )
        {
            info = (FileInfo*) tmp->nValue;
            if( info->m_nUserId == rq->m_nUserId && info->m_nFileID == rq->m_nFileId )
            {
                break;
            }
            tmp = tmp->pNext;
        }


        if( info )
        {
            //写入
            nLen = fwrite(rq->m_szFileContent,1 , rq->m_nBlockLen  , info->pFile);

            //rs.m_nBlockLen = nLen;
            info->m_nPos += nLen;
            //文件结束关闭
            if(  rq->m_nBlockLen < MAX_CONTENT_LEN || info->m_nPos >=  info->m_nFileSize )
            {
                //关闭文件 , 删除节点
                fclose ( info->pFile );

                if( strcmp( info->m_szFileType , "jpg") != 0)
                {
                    STRU_UPLOAD_RS rs;
                    rs.m_nType = DEF_PACK_UPLOAD_RS;
                    rs.m_nResult = 1;
                    //信息写到数据库中
                    char szsql[_DEF_SQLIEN];
                    bzero(szsql,sizeof(szsql));

                    char * picName = GetPicNameOfVideo( info->m_szFileName);
                    char* picPath =  GetPicNameOfVideo( info->m_szFilePath);
                    char rtmp[MAX_PATH] = {0};
                    sprintf(rtmp , "//%s/%s",info->m_UserName , info->m_szFileName);
                    sprintf(szsql,"insert into t_VideoInfo (userId, videoName , picName, videoPath, picPath , rtmp ,food , funny ,ennegy , dance , music , video , outside , edu ,hotdegree)values(%d,'%s','%s','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,0);"
                    ,info->m_nUserId , info->m_szFileName , picName ,info->m_szFilePath , picPath , rtmp , info->m_Hobby[0],info->m_Hobby[1],info->m_Hobby[2],info->m_Hobby[3],info->m_Hobby[4],info->m_Hobby[5],info->m_Hobby[6],info->m_Hobby[7]);
                    printf("%s\n",szsql);
                    if(UpdataMysql(szsql) == FALSE)
                    {
                        err_str("Updata MySql Failed...",-1);
                    }
                    free(picName);
                    free(picPath);
                    SendData(clientfd,(char*)&rs,sizeof(rs));
                }
                //如果是video 返回一个确认
                q_DeleteNode( FileQueue, info);
                free(  info );
                info = NULL;
            }
        }
}


void GetDownloadList( Queue*  plist,  int userId)
{
  //  根據id 獲取喜好， 推薦生成 列表，播放   /id -->

    char szsql[_DEF_SQLIEN];
	bzero(szsql,sizeof(szsql));
	Queue* pQueue = NULL;
	q_Init(&pQueue);
	char * szbuf = 0;
	int nCount = 0 ;

	//判断角色是否存在
	sprintf(szsql,"select count(videoId) from t_VideoInfo where t_VideoInfo.videoId not in ( select t_UserRecv.videoId from t_UserRecv where t_UserRecv.UserId = %d );",userId);
	printf("%s\n",szsql);
	if(SelectMysql(szsql,1,pQueue) == FALSE)
	{
		err_str("SelectMySql Falied:",-1);
		q_Destroy(&pQueue);
		return;
	}
    else{
		szbuf = (char*)q_Pop(pQueue);
        nCount = atoi(szbuf);
    }
    if( nCount < 10 )
    {
        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"delete from t_UserRecv  where  userId = %d ;",userId);
        printf("%s\n",szsql);
        if(UpdataMysql(szsql) == FALSE)
        {
            err_str("Updata MySql Failed...",-1);
        }
    }
    bzero(szsql,sizeof(szsql));
    sprintf(szsql,"select videoId , picName , picPath , rtmp from t_VideoInfo where t_VideoInfo.videoId not in ( select t_UserRecv.videoId from t_UserRecv where t_UserRecv.userId = %d );",userId);
    printf("%s\n",szsql);
	if(SelectMysql(szsql,4,pQueue) == FALSE)
	{
		err_str("SelectMySql Falied:",-1);
		q_Destroy(&pQueue);
		return;
	}
    else{
         //热度推荐
        for( int i = 1 ; i <= 10 ; i++ )
        {
            FileInfo * pInfo = (FileInfo *)malloc(sizeof(FileInfo )) ;
            pInfo->m_nPos = 0;
            pInfo->m_nFileID = i;

            pInfo->m_nUserId = userId;
            szbuf = (char*)q_Pop(pQueue);
            nCount = atoi(szbuf);
            pInfo->m_VideoID = nCount;
            szbuf = (char*)q_Pop(pQueue);
            strcpy( pInfo->m_szFileName ,szbuf);
            szbuf = (char*)q_Pop(pQueue);
            strcpy( pInfo->m_szFilePath, szbuf);
            szbuf = (char*)q_Pop(pQueue);
            strcpy( pInfo->m_szRtmp , szbuf);
            FILE* pFile = fopen(pInfo->m_szFilePath , "r");
            fseek(pFile,0 , SEEK_END);
            pInfo->m_nFileSize =  ftell(pFile);
            fseek(pFile , 0 , SEEK_SET);
            fclose(pFile);
            pInfo->pFile = 0;
            q_Push(plist,(void*)pInfo);
            bzero(szsql,sizeof(szsql));
            sprintf(szsql,"insert into t_UserRecv values(%d ,%d);",userId,pInfo->m_VideoID );
            printf("%s\n",szsql);
            if(UpdataMysql(szsql) == FALSE)
            {
                err_str("Updata MySql Failed...",-1);
            }
        }
    }




//
//    pInfo = (FileInfo *)malloc(sizeof(FileInfo )) ;
//    pInfo->m_nPos = 0;
//    pInfo->m_nFileID = 2;
//    pInfo->m_nFileSize = 13724;
//    pInfo->m_nUserId = userId;
//    strcpy( pInfo->m_szFileName ,"2.jpg");
//    strcpy( pInfo->m_szFilePath, "/home/colin/Video/img/2.jpg");
//    strcpy( pInfo->m_szRtmp , "2.flv");
//    pInfo->pFile = 0;
//    q_Push(plist,(void*)pInfo);
//
//    pInfo = (FileInfo *)malloc(sizeof(FileInfo )) ;
//    pInfo->m_nPos = 0;
//    pInfo->m_nFileID = 3;
//    pInfo->m_nFileSize = 27430;
//    pInfo->m_nUserId = userId;
//    strcpy( pInfo->m_szFileName ,"3.jpg");
//    strcpy( pInfo->m_szFilePath, "/home/colin/Video/img/3.jpg");
//    strcpy( pInfo->m_szRtmp , "3.flv");
//    pInfo->pFile = 0;
//    q_Push(plist,(void*)pInfo);


}


void DownloadFileBlockRs(int clientfd,char*szbuf)
{
    STRU_DOWNLOAD_FILEBLOCK_RS *psufr = (STRU_DOWNLOAD_FILEBLOCK_RS *)szbuf;

    Myqueue * tmp = FileQueue->pHead;
    FileInfo * pInfo = 0;
    while(tmp)
    {
        if( ((FileInfo*)tmp ->nValue)->m_nFileID == psufr->m_nFileId
        && ((FileInfo*)tmp ->nValue)->m_nUserId ==  psufr->m_nUserId )
        {
            pInfo = (FileInfo*)tmp ->nValue;
            break;
        }
        tmp = tmp->pNext;
    }
    if( !pInfo ) return;

	STRU_DOWNLOAD_FILEBLOCK_RQ sufr;
	sufr.m_nType = DEF_PACK_DOWNLOAD_FILEBLOCK_RQ;
	sufr.m_nFileId = pInfo->m_nFileID;
	sufr.m_nUserId = pInfo->m_nUserId;
	//如果文件块客户端接收失败，再次发送
	if(psufr->m_nResult == _downloadfileblock_fail)
	{
		//移动文件指针pInfo->m_nDownLoadSize
		fseeko64(pInfo->pFile,pInfo->m_nPos,SEEK_SET);
		int64_t nRelReadNum = (int64_t)fread(sufr.m_szFileContent,1,MAX_CONTENT_LEN,pInfo->pFile);
		sufr.m_nBlockLen = nRelReadNum;
		//读文件内容并发送
		SendData(clientfd,(char*)&sufr,sizeof(sufr));
		return;
	}
	pInfo->m_nPos += psufr->m_nBlockLen;

	if(pInfo->m_nPos == pInfo->m_nFileSize)
	{
		fclose(pInfo->pFile);

		Myqueue * tmp = FileQueue->pHead;
		if( ((FileInfo*)tmp->nValue) == pInfo )
		{
            tmp = q_Pop( FileQueue );
		}else{
            while(tmp)
            {
                if(tmp->pNext && ((FileInfo*)tmp->pNext->nValue)  == pInfo )
                {
                    tmp = tmp->pNext->pNext;
                    break;
                }
                tmp = tmp->pNext;
            }
		}
        free( pInfo );
		return;
	}
	else
	{
		 	int64_t nRelReadNum = (int64_t)fread(sufr.m_szFileContent,1,MAX_CONTENT_LEN,pInfo->pFile);
			sufr.m_nBlockLen = nRelReadNum;
			printf("m_nBlockLen:%d\n",sufr.m_nBlockLen);
	}
	SendData(clientfd,(char*)&sufr,sizeof(sufr));

}

void PressLikeRq(int clientfd,char*szbuf)
{
    STRU_PRESSLIKE_RQ * rq = (STRU_PRESSLIKE_RQ *)szbuf;

    char szsql[_DEF_SQLIEN];
	bzero(szsql,sizeof(szsql));

	sprintf(szsql,"insert into t_UserAction values( %d,%d);",rq->m_nUserId , rq->m_nVideoId);
    printf("%s\n",szsql);
    if(UpdataMysql(szsql) == FALSE)
    {
        err_str("Updata MySql Failed...",-1);
        return;
    }
    //更新视频热度
    bzero(szsql,sizeof(szsql));
    sprintf(szsql,"update t_VideoInfo set hotdegree = hotdegree +1 where videoId =%d;" , rq->m_nVideoId);
    printf("%s\n",szsql);
    if(UpdataMysql(szsql) == FALSE)
    {
        err_str("Updata MySql Failed...",-1);
        return;
    }
    //取出影片类型 取出用户喜好， 给用户喜好加分
    bzero(szsql,sizeof(szsql));
    Queue* pQueue = NULL;
	q_Init(&pQueue);
	char * szChar = 0;

	int food , funny ,ennegy ,dance , music,  video,  outside , edu ;
	int food1 , funny1 ,ennegy1 ,dance1 , music1,  video1,  outside1 , edu1;

	sprintf(szsql,"select food , funny ,ennegy ,dance , music,  video,  outside , edu from t_VideoInfo where videoId = %d;", rq->m_nVideoId);
    printf("%s\n",szsql);
    if(SelectMysql(szsql,8,pQueue) == FALSE)
    {
        err_str("SelectMysql Failed...",-1);
        q_Destroy(&pQueue);
        return;
    }else{
        szChar = (char*)q_Pop(pQueue);
        food = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        funny = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        ennegy = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        dance = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        music = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        video = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        outside = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        edu = atoi(szChar);
    }
    bzero(szsql,sizeof(szsql));
    sprintf(szsql,"select food , funny ,ennegy ,dance , music,  video,  outside , edu from t_UserData where id = %d;", rq->m_nUserId);
    printf("%s\n",szsql);
    if(SelectMysql(szsql,8,pQueue) == FALSE)
    {
        err_str("SelectMysql Failed...",-1);
        q_Destroy(&pQueue);
        return;
    }else{
        szChar = (char*)q_Pop(pQueue);
        food1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        funny1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        ennegy1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        dance1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        music1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        video1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        outside1 = atoi(szChar);
        szChar = (char*)q_Pop(pQueue);
        edu1 = atoi(szChar);
    }
        food   += food1;
        funny  += funny1;
        ennegy += ennegy1;
        dance  += dance1;
        music  += music1;
        video  += video1;
        outside += outside1;
        edu    += edu1;

        bzero(szsql,sizeof(szsql));
        sprintf(szsql,"update t_UserData set food =%d , funny =%d,ennegy =%d,dance=%d , music=%d,  video=%d,  outside=%d , edu=%d where id = %d;"
        ,food,funny,ennegy , dance,music , video , outside , edu , rq->m_nUserId);
        printf("%s\n",szsql);
        if(UpdataMysql(szsql) == FALSE)
        {
            err_str("Updata MySql Failed...",-1);
        }
}


