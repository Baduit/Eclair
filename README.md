# Eclair
## Description
It is a C++ header only library to call method of an object in a thread safe way without using extra code by only using a wrapper object, the wrapper object can access all method/attributes via the operator -> and lock use a mutex which will be locked/unlocked at the right time.

## Example
```C++
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
        // Increment the value accessing the attribute twice in the same statement
				my_struct->value = my_struct->value + 1;
        // Call the method print (the value might has been updated in another thread between this line and the line above)
				my_struct->print();
			}
		};

  // Start the threads
	std::thread a(cb);
	std::thread b(cb);
	std::thread c(cb);

  // Wait 5s
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5s);

  // Stop the threads
	run = false;

  // Join all the threads
	a.join();
	b.join();
	c.join();

	std::cout << "Final value = " << my_struct->value << std::endl;
}
```

## Requirements
A C++20 compiler.
It could be downgraded if we remove the concepts and make the comparisons operators the old way in the wrapper.

## How does it work ?
It uses the fact that [the operator-> if it does not return a pointer return the result of the operator-> of the returned object](https://en.cppreference.com/w/cpp/language/operators#Restrictions).
The Eclair::Value class operate as a simple wrapper with a mutex, it has its operator-> overloaded to return a temporary Eclair::Middleware with a pointer to the wrapped value and a reference to the wrapper mutex then the middleware lock the mutex in its constructor (using a std::lock_guard). It has an operator-> that return the pointer to the wrapped value. When the temporary middleware is destroyed the mutex is automatically unlocked.

## Why is a std::recursive_mutex by default ?
The mutex is not unlock when the destructor of the temporary object holding the lock is destroyed and not just after the action with the operator-> is finished, therefore if there are 2 uses of the operator-> in the same line it would cause a deadlock.

## Limitations
Mutexes are not a magic solution to make a multithreaded program, there are risk of deadlock (really bad) and it does not scale if a lot of thread must access the data. A good multithreaded should avoid sharing ressource and if information is needed across multiple thread, passing message between your thread is better. (and there is a lot of good ressource for it on the web) I made this library because I find the idea interesting and it has a really cool syntax.

## Why is it named "éclair" ?
Because it is a very good bakery with the good stuff on top of it (the icing) and inside it (the custard cream).
