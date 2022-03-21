#include <core.h>

#include <string>

int main(int argc, char *argv[])
{
	bool makeDaemon = true;
	bool testMode = false;
	if(argc == 2)
	{
		std::string arg2 = std::string(argv[1]);
		if(arg2 == "--no-daemon")
		{
			makeDaemon = false;
		}
		else if(arg2 == "--test")
		{
			makeDaemon = false;
			testMode = true;
		}
	}
	if(makeDaemon)
	{
		pid_t pid, sid;
		pid = fork();
		if(pid < 0)
		{
			exit(EXIT_FAILURE);
		}
		if(pid > 0)
		{
			exit(EXIT_SUCCESS);
		}
		umask(0);
		sid = setsid();
		if(sid < 0)
		{
			exit(EXIT_FAILURE);
		}
		if((chdir("/")) < 0)
		{
			exit(EXIT_FAILURE);
		}
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	for(;;)
	{
		Core* core = Core::getInstance();
		if(testMode)
		{
			core->setTestPlateString("X777XX178");
		}
		core->startCapture();
		core->joinVideoCaptureThread();
		Core::releaseInstance();
	}

	return 0;
}
