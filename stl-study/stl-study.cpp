#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "vector/study_vector.hpp"
#include "vector/vector_test.hpp"

#include "list/study_list.hpp"
#include "list/list_test.h"

struct Base
{
    Base() = default;
    Base(const Base& rhs) = default;
    Base& operator=(const Base& rhs) = default;
    Base(int i) : _i(i) {}
    virtual ~Base() = default;
    virtual void foo() = 0;

protected:
    int _i = 0;
};

struct Drived : public Base
{
    Drived() = default;
    ~Drived() override
    {
        _i = -1;
    }
    Drived(const Drived& rhs) = default;
    Drived(Drived&& rhs) noexcept
    {
        _i = rhs._i;
        rhs._i = -1;
    }
    Drived& operator=(const Drived& rhs) = default;
    Drived& operator=(Drived&& rhs) noexcept
    {
        _i = rhs._i;
        rhs._i = -1;
        return *this;
    }
    Drived(int i) : Base(i) {}
    void foo() override
    {
        std::cout << _i;
    }
};


static void vector_test()
{
    run_vector_test<mst::vector<int>, int>({ 1, 2, 3, 4, 5 });
    run_vector_test<mst::vector<std::string>, std::string>({ "Apple", "Banana", "Cherry" });
    Tracker::alive_count = 0;
    {
        run_vector_test<mst::vector<Tracker>, Tracker>({ Tracker(1), Tracker(2) });
    }
    assert(Tracker::alive_count == 0);
    std::cout << "Tracker Memory Leak Check: Pass (All objects destroyed)\n";

    mst::vector<int> test{ 1,2,3,4,5 };
    auto iter = test.begin();
    std::cout << *iter << std::endl;
    auto iter2 = iter + 2;
    std::cout << *iter2 << std::endl;
    auto iter1 = iter2 - 1;
    std::cout << *iter1 << std::endl;
    for (auto iter = test.begin(); iter != test.end(); iter++)
    {
        std::cout << *iter << ", ";
    }
    std::cout << std::endl;
    *iter2 = 0;
    test.erase(iter2);

    const auto& cTest = test;
    for (auto citer = cTest.cbegin(); citer != cTest.cend(); citer++)
    {
        std::cout << *citer << ", ";
    }
    std::cout << std::endl;

    {
        mst::vector<int> insertTest;
        auto iter = insertTest.insert(insertTest.begin(), 0);
        iter = insertTest.insert(insertTest.end(), 2);
        insertTest.insert(iter, 1);

        for (auto& i : insertTest)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }

    {
        mst::vector<int> insertTest;
        auto iter = insertTest.insert(insertTest.begin(), { 0, 1, 6, 7, 8 });
        insertTest.insert(iter + 2, { 2, 3, 4, 5 });

        for (auto& i : insertTest)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }

    {
        mst::vector<Drived> insertTest;
        auto iter = insertTest.insert(insertTest.begin(), { 0, 1, 6, 7, 8 });
        insertTest.insert(iter + 2, { 2, 3, 4, 5 });
        for (auto& item : insertTest)
        {
            item.foo();
            std::cout << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nAll Tests Finished Successfully!\n";
}

static void list_test()
{
    mst::list<int> mlt;
    mst::list<int>::iterator fiveIter;
    for (int i = 1; i <= 10; i++)
    {
        mlt.push_back(i);
        if (i == 5) 
            fiveIter = --mlt.end();
    }
    fiveIter = mlt.insert(fiveIter, 55);

    const mst::list<int>& cmlt = mlt;
    for (auto iter = cmlt.cbegin(); iter != cmlt.cend(); ++iter)
    {
        std::cout << *iter << std::endl;
    }
    mlt.erase(fiveIter);
    *fiveIter;

    const size_t size = mlt.size();
    for (int i = 1; i <= size; i++)
    {
        std::cout << mlt.size() << ", ";
        mlt.pop_back();
    }
    std::cout << mlt.size();

    mst::list<int> copySrc{ 1,2,3,4,5 };
    mst::list<int> copyTarget;
    copyTarget = copySrc;
    mst::list<int> moveTarget;
    moveTarget = std::move(copySrc);

    copyTarget.clear();
    moveTarget.clear();
    copySrc.clear();

    std::cout << std::endl;
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    vector_test();
    //list_test();

    return 0;
}
