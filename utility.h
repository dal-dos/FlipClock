//utility.h
#ifndef _UTILITY_H_
#define _UTILITY_H_

static void sleepForMs(long long delayInMs){
	const long long NS_PER_MS = 1000 * 1000;
 	const long long NS_PER_SECOND = 1000000000;
 	long long delayNs = delayInMs * NS_PER_MS;
 	int seconds = delayNs / NS_PER_SECOND;
 	int nanoseconds = delayNs % NS_PER_SECOND;
 	struct timespec reqDelay = {seconds, nanoseconds};
 	nanosleep(&reqDelay, (struct timespec *) NULL);
}

static int readLineFromFile(const char* fileName, char* buff, unsigned int maxLength)
{
	FILE *file = fopen(fileName, "r");
	int bytes_read = getline(&buff, &maxLength, file);
	fclose(file);
	return bytes_read;
}

#endif