#include <atomic>

typedef struct
{
	char			sDateTime [20];
	std::atomic<bool>	bReadyToFlush;
} LogEntries;

typedef struct
{
	LogEntries		leLogEntries [1] {};
} LogThreads;

