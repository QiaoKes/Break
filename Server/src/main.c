#include <TCPKernel.h>


int main(int argc,char *argv[])
{
	
	pthread_mutex_init(&alock,NULL);
	pthread_mutex_init(&rlock,NULL);
	if(Open() == TRUE)
	{
		while(1)
		{
			printf("Server Running...\n");
			sleep(_DEF_TIMEOUT*3);
		}
	}
	Close();



	return 0;
}
