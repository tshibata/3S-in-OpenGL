
namespace sssx
{

class Task
{
public:
	static Task closing;
	int priority = 0;
	Task() {}
	virtual ~Task() {}
	virtual void execute();
};

class TaskQueue
{
private:
	class Comparator
	{
	public:
		bool operator() (Task * a, Task * b);
	};
	int parallelism;
	std::thread * * threads;
	std::priority_queue<Task *, std::vector<Task *>, Comparator> queue;
	std::mutex mutex;
	std::condition_variable cond;
	void execute();
public:
	TaskQueue(int parallelism);
	~TaskQueue();
	void push(Task & task);
};

}

