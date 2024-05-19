#include "PCH/PCH.h"
#include "scheduler.h"

namespace he
{
	static scheduler* instance = nullptr;
	
	scheduler::scheduler()
	{
		std::unordered_set<thread_id> thread_id_set;
	
		uint8_t thread_count = (uint8_t)std::thread::hardware_concurrency();
		for (uint8_t i = 0; i < thread_count; ++i)
		{
			_threads.push_back(std::make_unique<thread>());
	
			thread_id_set.insert(_threads[i]->get_thread_id());
	
			HE_LOG(Core, ELogLevel::Info, "active thread: [%u]", _threads[i]->get_thread_id());
		}
	
		if (thread_id_set.size() != thread_count)
		{
			HE_LOG(Core, ELogLevel::Critical, "duplicate thread id");
			HE_ASSERT(false && "duplicate thread id");
		}
		else
		{
			HE_LOG(Core, ELogLevel::Info, "active thread total count: %d ", threadCount);
		}

		
		_bIsTaskRunning = false;
		_recommanded_thread_index = 0;
	}

	void scheduler::initialize(const scheduler_arguments& in_args)
	{
		if(instance != nullptr)
		{
			return;
		}

		instance = new scheduler();
	}
	
	void scheduler::shutdown()
	{
		delete instance;
		instance = nullptr;
	}
	
	scheduler& scheduler::get_instance()
	{
		HE_CHECK(instance);
		return *instance;
	}
	
	uint8_t scheduler::get_recommand_thread_index()
	{
		uint8_t result_index = INDEX_NONE;
		uint8_t threadCount = (uint8_t)_threads.size();
		
		while(result_index == INDEX_NONE)
		{
			for (uint8_t i = _recommanded_thread_index; i < threadCount; ++i)
			{
				if (_threads[i]->is_running())
				{
					continue;
				}
	
				result_index = i;
				break;
			}
		}

		_recommanded_thread_index = result_index;
		return result_index;
	}
}

