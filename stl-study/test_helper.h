#pragma once

#include "vector/study_vector.hpp"
#include "vector/vector_test.hpp"

#include "list/study_list.hpp"

#include "string/study_string.hpp"

struct VirtualBase
{
    VirtualBase() = default;
    VirtualBase(const VirtualBase& rhs) = default;
    VirtualBase& operator=(const VirtualBase& rhs) = default;
    VirtualBase(int i) : _i(i) {}
    virtual ~VirtualBase() = default;
    virtual void foo() const = 0;

protected:
    int _i = 0;
};

struct VirtualDrived : public VirtualBase
{
    VirtualDrived() = default;
    ~VirtualDrived() override
    {
        _i = -1;
    }
    VirtualDrived(const VirtualDrived& rhs) = default;
    VirtualDrived(VirtualDrived&& rhs) noexcept
    {
        _i = rhs._i;
        rhs._i = -1;
    }
    VirtualDrived& operator=(const VirtualDrived& rhs) = default;
    VirtualDrived& operator=(VirtualDrived&& rhs) noexcept
    {
        _i = rhs._i;
        rhs._i = -1;
        return *this;
    }
    VirtualDrived(int i) : VirtualBase(i) {}
    void foo() const override
    {
        std::cout << _i;
    }
};

namespace mst
{
    void vector_test();
    void list_test();
    void string_test();
}
