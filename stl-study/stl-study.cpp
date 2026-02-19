#include "vector/study_vector.hpp"
#include "vector/vector_test.hpp"

template <typename T>
static void vector_test_entry()
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

int main()
{
    vector_test_entry<int>();

    std::vector<int> std;

    return 0;
}
