#include <windows.h>
#include <process.h>
#include <stdio.h>

UINT WINAPI worker(void* arg)
{
	printf("Hello in worker\n");
	return 0;
}

int main()
{
	HANDLE thread;
	thread = _beginthreadex(NULL, 0, worker, NULL, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	return 0;
}