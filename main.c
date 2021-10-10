/*
 * @Description: 
 * @Author: JesonKid
 * @version: 
 * @Date: 2021-10-10 13:11:53
 * @LastEditors: JesonKid
 * @LastEditTime: 2021-10-10 21:50:53
 */
#include "time.h"
#include "stdint.h"
#include "stdio.h"
#include "bget/bget.h"
#include "windows.h"

#define POOL_SIZE    20*1024

int main()
{
	LARGE_INTEGER time_start;	//开始时间
	LARGE_INTEGER time_over;	//结束时间
	double dqFreq;		//计时器频率
	LARGE_INTEGER f;	//计时器频率
	
	void* pool = NULL;
	double total_allocate_time = 0;
	double total_release_time = 0;
	double total_time = 0;

	pool = malloc(POOL_SIZE);

	bpool(pool, (bufsize)POOL_SIZE);
	bpoolv(pool);

	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart; //获取主频
	printf("------------------------------------------------------\n");
	printf("------------bget allocate--------\n");
	for (uint16_t i = 0; i < 1000; i++)
	{
		QueryPerformanceCounter(&time_start);	//计时开始
		//申请1K的内存空间
		void* bptr = bget((bufsize)1024);
		while (bptr == NULL);
		QueryPerformanceCounter(&time_over);	//计时结束
		total_allocate_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_allocate_time：%fus\n", total_allocate_time);

		QueryPerformanceCounter(&time_start);	//计时开始
        //释放1K的内存空间
		brel(bptr);
		QueryPerformanceCounter(&time_over);	//计时结束
		total_release_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_release_time：%fus\n", total_release_time);
		total_time += total_allocate_time + total_release_time;
	}


	//乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
	printf("------------------------------------------------------\n");
	printf("\ntotal_time：%fus\n", total_time);
	total_time = 0;
	printf("------------malloc allocate--------\n");
	for (uint16_t i = 0; i < 1000; i++)
	{
		QueryPerformanceCounter(&time_start);	//计时开始
		//申请1K的内存空间
		void* bptr = malloc(1024);
		while (bptr == NULL);
		QueryPerformanceCounter(&time_over);	//计时结束
		total_allocate_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_allocate_time：%fus\n", total_allocate_time);

		QueryPerformanceCounter(&time_start);	//计时开始
		//释放1K的内存空间
		free(bptr);
		QueryPerformanceCounter(&time_over);	//计时结束
		total_release_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
		printf("\ntotal_release_time：%fus\n", total_release_time);
		total_time += total_allocate_time + total_release_time;
	}


	//乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
	printf("\ntotal_time：%fus\n", total_time);

	return 0;
}

