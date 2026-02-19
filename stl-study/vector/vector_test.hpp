#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// [Tracker는 그대로 유지 - 메모리 체크용]
struct Tracker
{
    inline static int alive_count = 0;
    int id;
    Tracker() : id(0) { alive_count++; }
    Tracker(int i) : id(i) { alive_count++; }
    Tracker(const Tracker& o) : id(o.id) { alive_count++; }
    Tracker(Tracker&& o) noexcept : id(o.id) { o.id = -1; alive_count++; }
    ~Tracker() { alive_count--; }
    Tracker& operator=(const Tracker& o) { id = o.id; return *this; }
    bool operator==(const Tracker& o) const { return id == o.id; }
};

// [테스트 함수: 타입 T를 인자로 받음]
template <typename VecType, typename T>
void run_vector_test(std::initializer_list<T> init_data)
{
    std::cout << "--- Testing Vector with type: " << typeid(T).name() << " ---\n";

    // 1. Initializer List 생성자 & Index 접근
    VecType v(init_data);
    assert(v.size() == init_data.size());

    // 데이터 일치 확인
    size_t idx = 0;
    for (const auto& expected : init_data) {
        assert(v[idx] == expected);
        idx++;
    }
    std::cout << "1. Initializer list & Access: Pass\n";

    // 2. push_back & pop_back
    size_t prev_size = v.size();
    T extra = *init_data.begin(); // 첫 번째 원소 복사해서 사용
    v.push_back(extra);
    assert(v.size() == prev_size + 1);
    assert(v[v.size() - 1] == extra);

    v.pop_back();
    assert(v.size() == prev_size);
    std::cout << "2. Push & Pop: Pass\n";

    // 3. Capacity, Reserve, Resize
    size_t old_cap = v.capacity();
    v.reserve(old_cap + 10);
    assert(v.capacity() >= old_cap + 10);

    v.resize(v.size() + 5);
    assert(v.size() == prev_size + 5);

    v.resize(prev_size); // 원래대로
    assert(v.size() == prev_size);
    std::cout << "3. Capacity, Reserve, Resize: Pass\n";

    // 4. 복사/이동 시멘틱
    {
        VecType v_copy = v; // 복사 생성자
        assert(v_copy.size() == v.size());
        assert(v_copy[0] == v[0]);

        VecType v_move = std::move(v_copy); // 이동 생성자
        assert(v_move.size() == v.size());
    }
    std::cout << "4. Copy & Move: Pass\n";

    // 5. Empty & Clear
    assert(!v.empty());
    v.clear();
    assert(v.empty());
    assert(v.size() == 0);
    std::cout << "5. Empty & Clear: Pass\n\n";
}
