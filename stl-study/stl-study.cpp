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

static void vector_test()
{
    // 1. stl 테스트
    run_vector_test<std::vector<int>, int>({ 1, 2, 3, 4, 5 });
    run_vector_test<std::vector<std::string>, std::string>({ "Apple", "Banana", "Cherry" });
    Tracker::alive_count = 0;
    {
        run_vector_test<std::vector<Tracker>, Tracker>({ Tracker(1), Tracker(2) });
    }
    assert(Tracker::alive_count == 0);
    std::cout << "Tracker Memory Leak Check: Pass (All objects destroyed)\n";
    std::cout << "----------------------------------------------------------\n";

    // 2. 내 타입 테스트
    run_vector_test<mst::vector<int>, int>({ 1, 2, 3, 4, 5 });
    run_vector_test<mst::vector<std::string>, std::string>({ "Apple", "Banana", "Cherry" });
    Tracker::alive_count = 0;
    {
        run_vector_test<mst::vector<Tracker>, Tracker>({ Tracker(1), Tracker(2) });
    }
    assert(Tracker::alive_count == 0);
    std::cout << "Tracker Memory Leak Check: Pass (All objects destroyed)\n";

    std::cout << "\nAll Tests Finished Successfully!\n";
}

static void list_test()
{
    std::list<int> slt;

    mst::list<int> mlt;
    for (int i = 1; i <= 10; i++)
    {
        mlt.push_front(i);
    }
   
    auto iter = mlt.begin();
    for (auto iter = mlt.begin(); iter != mlt.end(); ++iter)
    {
        std::cout << *iter << std::endl;
    }
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //vector_test();
    list_test();

    return 0;
}
