#pragma once

#include <compare>
#include <concepts>
#include <type_traits>
#include <mutex>

namespace Eclair
{

template <typename T, typename... Args>
concept ConstructibleFrom =
	requires (Args&&... args)
	{
		{ T(std::forward<Args>(args)...) } -> std::same_as<T>;
	};

template <typename T, typename U>
concept AssignableFrom =
	requires (T t, U u)
	{
		{ t = u };
	};

template <typename T>
concept Pointer = std::is_pointer_v<T>;


template <Pointer T, typename M>
class Middleware
{
	public:
		Middleware(T v, M& m):
			_value(v),
			_lock(m)
		{}

	T operator->() { return _value; }

	private:
		T _value;
		std::lock_guard<M> _lock;
};


template <typename T, typename M = std::recursive_mutex>
class Value
{
	public:
		using ValueType = T;

		Value() = default;

		Value(const Value&) = delete;
		Value& operator=(const Value&) = delete;

		Value(Value&&) = delete;
		Value& operator=(Value&&) = delete;

		template <typename... Args> requires ConstructibleFrom<T, Args...>
		explicit Value(Args&&... args):
			_value(std::forward<Args>(args)...)
		{}

		template <typename U> requires AssignableFrom<T, U>
		Value& operator=(U&& other)
		{
			std::lock_guard lock(_mutex);
			_value = std::forward<U>(other);
			return *this;
		}

		Middleware<T*, M> operator->() { return Middleware<T*, M>(&_value, _mutex); }
		Middleware<const T*, M> operator->() const { return Middleware<const T*, M>(&_value, _mutex); }

	private:
		mutable M _mutex;
		ValueType _value;
};


} // namespace Eclair