#include "time.h"
#include "stdint.h"
#include "stdio.h"
#include "bget/bget.h"
#include "windows.h"

#define POOL_SIZE    10*1024

int main()
{
	LARGE_INTEGER time_start;	//��ʼʱ��
	LARGE_INTEGER time_over;	//����ʱ��
	double dqFreq;		//��ʱ��Ƶ��
	LARGE_INTEGER f;	//��ʱ��Ƶ��
	void* pool = NULL;
	double total_allocate_time = 0;
	double total_release_time = 0;
	double total_time = 0;

	pool = malloc(POOL_SIZE);

	bpool(pool, (bufsize)POOL_SIZE);
	bpoolv(pool);

	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart; //��ȡ��Ƶ
	printf("------------------------------------------------------\n");
	printf("------------bget allocate--------\n");
	for (uint16_t i = 0; i < 1000; i++)
	{
		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
		//����1K���ڴ�ռ�
		void* bptr = bget((bufsize)1024);
		while (bptr == NULL);
		QueryPerformanceCounter(&time_over);	//��ʱ����
		total_allocate_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_allocate_time��%fus\n", total_allocate_time);

		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
        //�ͷ�1K���ڴ�ռ�
		brel(bptr);
		QueryPerformanceCounter(&time_over);	//��ʱ����
		total_release_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_release_time��%fus\n", total_release_time);
		total_time += total_allocate_time + total_release_time;
	}


	//����1000000�ѵ�λ���뻯Ϊ΢�룬����Ϊ1000 000/��cpu��Ƶ��΢��
	printf("------------------------------------------------------\n");
	printf("\ntotal_time��%fus\n", total_time);
	total_time = 0;
	printf("------------malloc allocate--------\n");
	for (uint16_t i = 0; i < 1000; i++)
	{
		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
		//����1K���ڴ�ռ�
		void* bptr = malloc(1024);
		while (bptr == NULL);
		QueryPerformanceCounter(&time_over);	//��ʱ����
		total_allocate_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_allocate_time��%fus\n", total_allocate_time);

		QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
		//�ͷ�1K���ڴ�ռ�
		free(bptr);
		QueryPerformanceCounter(&time_over);	//��ʱ����
		total_release_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_release_time��%fus\n", total_release_time);
		total_time += total_allocate_time + total_release_time;
	}


	//����1000000�ѵ�λ���뻯Ϊ΢�룬����Ϊ1000 000/��cpu��Ƶ��΢��
	printf("\ntotal_time��%fus\n", total_time);

	return 0;
}

