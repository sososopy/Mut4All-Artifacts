#pragma once

#include <cstddef>
#include <deque>
#include <stdexcept>

template <typename data_t>
struct fenwick_t
{
    std::deque<data_t> datas;
    std::deque<data_t> original_datas;

    fenwick_t() = default;
    fenwick_t(size_t size) : datas(size, data_t()) {}

    size_t size() const
    {
        return datas.size();
    }

    size_t lowbit(size_t x) const
    {
        return x & -x;
    }

    void modify(size_t idx, data_t value)
    {
        if (idx >= datas.size())
        {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = idx + 1; i <= datas.size(); i += lowbit(i))
        {
            datas[i - 1] += value; // Notice for the 0-start and 1-start index difference
        }
        original_datas[idx] += value;
    }

    data_t query(size_t to)
    {
        if (to >= datas.size())
        {
            throw std::out_of_range("Index out of range");
        }
        data_t res = data_t();
        for (size_t i = to + 1; i > 0; i -= lowbit(i))
        {
            res += datas[i - 1]; // Notice for the 0-start and 1
        }
        return res;
    }

    data_t query(size_t from, size_t to)
    {
        if (from > to)
        {
            throw std::out_of_range("From index cannot be greater than To index");
        }
        return query(to) - query(from - 1);
    }

    data_t query_at(size_t idx)
    {
        if (idx >= datas.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return query(idx, idx);
    }

    void push_back(data_t value)
    {
        datas.push_back(data_t());
        original_datas.push_back(value);
        modify(datas.size() - 1, value);
    }

    size_t lower_bound_traditional(data_t value)
    {
        if (value < data_t())
        {
            return 0;
        }

        // Linear search for lower bound
        data_t sum = data_t();
        for (size_t i = 0; i < datas.size(); ++i)
        {
            sum += original_datas[i];
            if (sum >= value)
            {
                return i;
            }
        }
        return datas.size();
    }

    size_t lower_bound(data_t value)
    {
        if (value < data_t())
        {
            return 0;
        }

        // Use binary search to speed up the search
        size_t left  = 0;
        size_t right = datas.size() - 1;
        while (left < right)
        {
            size_t mid = left + (right - left) / 2;
            if (query(mid) < value)
            {
                left = mid + 1;
            }
            else
            {
                right = mid;
            }
        }
        if (left < datas.size() && query(left) >= value)
        {
            return left;
        }
        return datas.size(); // Not found
    }
};