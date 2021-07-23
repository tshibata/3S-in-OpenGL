#include <cstdio>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "Task.h"

Task Task::closing;

void Task::execute()
{
}

bool TaskQueue::Comparator::operator() (Task * a, Task * b)
{
   return a->priority < b->priority;
}

TaskQueue::TaskQueue(int parallelism) : parallelism(parallelism)
{
	threads = new std::thread *[parallelism];
	for (int i = 0; i < parallelism; i++)
	{
		threads[i] = new std::thread(& TaskQueue::execute, this);
	}
}

TaskQueue::~TaskQueue()
{
	push(Task::closing);
	for (int i = 0; i < parallelism; i++)
	{
		threads[i]->join();
		delete threads[i];
	}
	while (queue.size() != 0)
	{
		queue.pop();
	}
	delete [] threads;
}

void TaskQueue::push(Task & task)
{
	std::unique_lock<std::mutex> lock(mutex);
	queue.push(& task);
	cond.notify_one();
}

void TaskQueue::execute()
{
	while (true)
	{
		Task * task;
		{
			std::unique_lock<std::mutex> lock(mutex);
			if (queue.size() == 0)
			{
				cond.wait(lock);
			}
			task = queue.top();
			if (task == & Task::closing)
			{
				cond.notify_one();
				break;
			}
			queue.pop();
		}
		task->execute();
	}
}

