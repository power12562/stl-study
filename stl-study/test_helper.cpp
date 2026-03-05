#include "test_helper.h"

void mst::vector_test()
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
        mst::vector<VirtualDrived> insertTest;
        auto iter = insertTest.insert(insertTest.begin(), { 0, 1, 6, 7, 8 });
        insertTest.insert(iter + 2, { 2, 3, 4, 5 });
        for (auto& item : insertTest)
        {
            item.foo();
            std::cout << ", ";
        }
        std::cout << std::endl;
    }

    {
        mst::vector<std::unique_ptr<int>> ptrTest;
        std::unique_ptr<int> p1 = std::make_unique<int>(0);
        // ptrTest.push_back(p1); // 컴파일러 에러. copy 금지
        ptrTest.push_back(std::move(p1)); // move는 가능
        for (int i = 1; i <= 10; i++)
        {
            ptrTest.push_back(std::make_unique<int>(i)); // xvalue도 가능
        }

        for (auto& ptr : ptrTest)
        {
            std::cout << *ptr << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nAll Tests Finished Successfully!\n";
}

void mst::list_test()
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

    {
        constexpr const char* testStr[] = {
            "Apply",
            "Back",
            "Copy",
            "Delegate",
            "Element"
        };

        mst::list<std::string> strTest;
        for (auto& str : testStr)
        {
            strTest.push_back(str);
        }

        std::cout << "size :" << strTest.size() << std::endl;
        for (auto& item : strTest)
        {
            std::cout << item << ", ";
        }
        std::cout << std::endl;
    } 
}

void mst::string_test()
{
    {
        mst::string str("hello");
        mst::wstring wstr(L"world!");

        std::cout << str.data() << " ";
        std::wcout << wstr.data() << L"\n";

        std::cout << "str cap :" << str.capacity() << "\n";
        std::cout << "wstr cap :" << wstr.capacity() << "\n";

        str.clear();
        wstr.clear();
    }

    {
        mst::string str("abcdefghijklmnopqrstuvwxyz");
        mst::wstring wstr(L"이건 긴 문자열 테스트를 위한 임시 텍스트 입니다.");

        std::cout << str.data() << "\n";
        std::wcout << wstr.data() << L"\n";

        str.reserve(100);
        wstr.reserve(100);

        std::cout << "str cap :" << str.capacity() << "\n";
        std::cout << "wstr cap :" << wstr.capacity() << "\n";

        str.clear();
        wstr.clear();
    }

    {
        mst::string cpySrc = "abcdefghijklmnopqrstuvwxyz";
        mst::string cpyDst(cpySrc);

        std::cout << "cpy src :" << cpySrc << "\n";
        std::cout << "cpy dst :" << cpyDst << "\n";

        mst::string moveDst(std::move(cpySrc));

        std::cout << "cpy src :" << cpySrc << "\n";
        std::cout << "move dst :"<< moveDst << "\n";

    }
}



