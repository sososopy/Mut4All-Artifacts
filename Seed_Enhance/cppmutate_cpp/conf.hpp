#pragma once

#include <json/value.h>
#include <memory>
#include <string>

#include "json/json.h"

template <typename T>
struct is_array_type : std::false_type
{};

template <typename U, typename Alloc>
struct is_array_type<std::vector<U, Alloc>> : std::true_type
{};

template <typename U, typename Alloc>
struct is_array_type<std::deque<U, Alloc>> : std::true_type
{};

template <typename T>
typename std::enable_if<is_array_type<T>::value, T>::type conf_get_impl(const Json::Value& root,
                                                                        const std::string& key)
{
    using Q = typename T::value_type;
    if (!root.isMember(key))
    {
        throw std::runtime_error("Key not found in configuration: " + key);
    }
    const Json::Value& value = root[key];
    if (!value.isArray())
    {
        throw std::runtime_error("Expected an array for key: " + key);
    }
    T vec;
    for (const auto& item : value)
    {
        vec.push_back(item.as<Q>());
    }
    return vec;
}

template <typename T>
typename std::enable_if<!is_array_type<T>::value, T>::type conf_get_impl(const Json::Value& root,
                                                                         const std::string& key)
{
    if (!root.isMember(key))
    {
        throw std::runtime_error("Key not found in configuration: " + key);
    }
    const Json::Value& value = root[key];
    return value.as<T>();
}

struct conf_t
{
    template <typename T>
    T operator[](const std::string& key)
    {
        return conf_get_impl<T>(root, key);
    }

    template <typename T>
    T get(const std::string& key)
    {
        return conf_get_impl<T>(root, key);
    }

    Json::Value root;
};

void    conf_init(const std::string& file_name);
conf_t& conf();