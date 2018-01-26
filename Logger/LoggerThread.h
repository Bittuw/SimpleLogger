#pragma once

#include <type_traits>
#include <memory>
#include <mutex>

#include "Data_types.hpp"

class LoggingThread {
public:
	LoggingThread(Messages_buffer_ref&, std::shared_ptr<std::mutex>&, std::shared_ptr<std::condition_variable>&, Destinations_list_ref&);
	static void startThread(Messages_buffer_ref&, std::shared_ptr<std::mutex>&, std::shared_ptr<std::condition_variable>&, std::shared_ptr<std::vector<Channal>>&);

	LoggingThread(const LoggingThread&) = delete;
	void* operator new[](size_t) = delete;
	void operator delete[](void*) = delete;
	void operator delete(void* pointer);
	
	static void atexti_flush();

	~LoggingThread();

private:
	explicit LoggingThread() {};
	void* operator new(size_t);

	Messages_buffer_ref _queue;
	std::shared_ptr<std::mutex> _mutex;
	std::shared_ptr<std::condition_variable> _wakeUp;

	Destinations_list_ref _destination;

	Commands_buffer_ref _buffer_commands;

	void RunningInThread();

	template <typename T>
	static void flush_command_buffer(T& buffer) {

		static_assert(std::is_same<T, Commands_buffer_ref>::value, "Not std::vector"); // Temp check

		if (!buffer->empty())
			for (auto command : *(buffer)) {
				static_assert(std::is_object<decltype(command->_message_delivered)>::value, "Stored type of vector do not have '_message_delivered' value!");
					if (!command->_message_delivered)
						command->work();
			}

		T empty_buffer;
		buffer.swap(empty_buffer);
	}

	template <typename T>
	static void flush_queue_buffer(T& buffer) {

		static_assert(std::is_same<T, Messages_buffer_ref>::value, "Not std::queue!"); // // Temp check

		if (!buffer->empty())
			while (!buffer->empty()) {
				auto command = buffer->front();
				LoggerThreadState::ref_queue_buffer->pop();
				command->work();
			}

		T empty_buffer;
		buffer.swap(empty_buffer);
	}

	template <typename T>
	static void flush_destination_buffers(T& destinations) {

		static_assert(std::is_same<T, Destinations_list_ref>::value, "Not destination type!");

		for (auto dist : *destinations) {
			dist.out_dist->flush();
			std::dynamic_pointer_cast<std::ofstream>(dist.out_dist)->close();
		}

		T empty_destinations;
		destinations.swap(empty_destinations);

	}

	static bool created;
};