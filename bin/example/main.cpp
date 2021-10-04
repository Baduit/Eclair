#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

#include <Eclair/Eclair.hpp>

struct SimpleStruct
{
	void print() const
	{
		std::cout << "My value is : " << value << std::endl;
	}

	int value = 0;
};

int main()
{
	Eclair::Value<SimpleStruct> my_struct;
	std::atomic<bool>  run{ true };

	auto cb =
		[&]()
		{
			while(run)
			{
				my_struct->value = my_struct->value + 1;
				my_struct->print();
			}
		};

	std::thread a(cb);
	std::thread b(cb);
	std::thread c(cb);

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5s);
	run = false;

	a.join();
	b.join();
	c.join();

	std::cout << "Final value = " << my_struct->value << std::endl;
}