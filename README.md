# Eclair
## Description
It is a C++ header only library to call method of an object in a thread safe way without using extra code by only using a wrapper object, the wrapper object can access all method/attributes via the operator -> and lock use a mutex which will be locked/unlocked at the right time.

## Example
```C++
int main()
{
  Eclair::Value<std::string> str;
  auto i = str->size(); // It lock a mutex, then call str.size() then unlock the mutex
}
```

## Requirements
A C++20 compiler.
It could be downgraded if we remove the concepts and make the comparisons operators the old way in the wrapper.

## How does it work ?
Dark magic (kidding)
TODO

## Limitations
TODO

## Why is it named "éclair" ?
Because it is a very good bakery with the good stuff inside it (the icing).
