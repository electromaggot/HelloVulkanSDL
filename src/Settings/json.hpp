//
// json.hpp
//	Application Settings
//
// THIS FILE IS A STUB!
//	If you have no need for Saved Settings, do nothing!
//
// However if you'd like to save Settings to JSON, replace this file
//	with https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
//		(https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp)
//	from https://github.com/nlohmann/json
//
// Created 6/27/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef json_h
#define json_h

#include <map>


namespace nlohmann
{
	namespace detail {
		enum class value_t { null };
	};

	template<template<typename U, typename V, typename... Args> class ObjectType = std::map,
		template<typename U, typename... Args> class ArrayType = std::vector,
		class StringType = std::string, class BooleanType = bool,
		class NumberIntegerType = std::int64_t,
		template<typename U> class AllocatorType = std::allocator >
	class basic_json
	{
	public:
		template<typename BasicJsonType>
		class json_ref {
		  public:
			using value_type = BasicJsonType;

			json_ref(std::initializer_list<json_ref> init)
				: owned_value(init), value_ref(&owned_value), is_rvalue(true)	{ }

			template <
				class... Args,
				enable_if_t<std::is_constructible<basic_json, Args...>::value, int> = 0 >
			json_ref(Args && ... args)
				: owned_value(std::forward<Args>(args)...), value_ref(&owned_value),
				  is_rvalue(true) {}

			json_ref(json_ref&&) = default;
			~json_ref() = default;

		  private:
			mutable value_type owned_value = nullptr;
			value_type* value_ref = nullptr;
			const bool is_rvalue;
		};

		using initializer_list_t = std::initializer_list<json_ref<basic_json>>;


		basic_json(initializer_list_t init)
		{ }

		basic_json() noexcept
		{ }

		template <typename BasicJsonType>
		basic_json(const BasicJsonType& val)
		{ }


		int operator[](const char*)	const {
			return -1;
		}

		friend std::ostream& operator<<(std::ostream& o, const basic_json& j) {
			return o;
		}

		friend std::istream& operator>>(std::istream& i, basic_json& j) {
		   return i;
		}
		// Input stream note: the original (nlohmann/json) version of the above method parses JSON when passed
		//	it, throwing exceptions on parse errors.  This stub isn't supposed to do anything, so won't throw,
		//	given valid JSON or not.  To ignore JSON handling, file or none, the below methods may be helpful:

		bool empty() {
			return true;
		}

		detail::value_t type() {
			return detail::value_t::null;
		}
	};

	using json = basic_json<>;

};

#endif	// json_h
