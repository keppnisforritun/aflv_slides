#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

class segment_tree_t {
    size_t n;
    struct storage_t {
        int32_t value;
        int32_t lazy;
    };
    mutable vector<storage_t> data;
    constexpr static size_t root{ 1ull };
    constexpr static size_t identity{ 0 };
    constexpr static size_t lazy_identity{ 0 };


    constexpr int32_t op(const int32_t &a, const int32_t &b) const {
        return a + b;
    }

    constexpr void push(int32_t &parent_lazy, int32_t &left_lazy, int32_t &right_lazy) const {
        left_lazy += parent_lazy;
        right_lazy += parent_lazy;
    }

    constexpr int32_t apply(const int32_t &value, const int32_t &lazy, const size_t left, const size_t right) const {
        return value + lazy * (right - left + 1);
    }

    const storage_t &get_identity() const {
        return data.at(0);
    }

    constexpr static size_t left_child(const size_t i) {
        return 2 * i;
    }

    constexpr static size_t right_child(const size_t i) {
        return 2 * i + 1;
    }

    void propagate(const size_t ind, const size_t left, const size_t right) const {
        if (left < right) {
            push(data[ind].lazy, data[left_child(ind)].lazy, data[right_child(ind)].lazy);
        }
        data[ind].value = apply(data[ind].value, data[ind].lazy, left, right);
        data[ind].lazy = get_identity().lazy;
    }

    int32_t query(const size_t l, const size_t r, const size_t ind, const size_t left, const size_t right) const {
        propagate(ind, left, right);
        if (l <= left && right <= r) {
            return data[ind].value;
        }
        else if (max(left, l) > min(right, r)) {
            return get_identity().value;
        }
        const auto mid{ midpoint(left, right) };
        const auto left_val{ query(l, r, left_child(ind), left, mid) };
        const auto right_val{ query(l, r, right_child(ind), mid + 1, right) };
        return op(left_val, right_val);
    }

    int32_t update(const size_t l, const size_t r, const int32_t &value, const size_t ind, const size_t left, const size_t right) {
        propagate(ind, left, right);
        if (l <= left && right <= r) {
            data[ind].lazy = value;
            propagate(ind, left, right);
            return data[ind].value;
        }
        else if (max(left, l) > min(right, r)) {
            return data[ind].value;
        }
        const auto mid{ midpoint(left, right) };
        const auto left_val{ update(l, r, value, left_child(ind), left, mid) };
        const auto right_val{ update(l, r, value, right_child(ind), mid + 1, right) };
        const auto new_value{ op(left_val, right_val) };
        return data[ind].value = new_value;
    }
public:
    segment_tree_t(const vector<int32_t> &source)
        : n(source.size()), data(max(static_cast<size_t>(1), 4 * n)) {
        data[0] = { identity, lazy_identity };
        if (source.size()) {
            build(source, root, 0, n - 1);
        }
    }

    int32_t build(const vector<int32_t> &source, const size_t ind, const size_t left, const size_t right) const {
        if (left == right) {
            return data[ind].value = source[left];
        }
        const auto mid{ midpoint(left, right) };
        return data[ind].value = op(build(source, left_child(ind), left, mid), build(source, right_child(ind), mid + 1, right));
    }

    int32_t query(const size_t l, const size_t r) const {
        return query(l, r, root, 0, n - 1);
    }

    int32_t update(const size_t l, const size_t r, const int32_t &value) {
        return update(l, r, value, root, 0, n - 1);
    }
};

constexpr int32_t update_op = 1;
constexpr int32_t query_op = 2;

int main() {
    int32_t n, q;
    cin >> n >> q;
    vector<int32_t> original(n);
    for (auto &x : original) {
        cin >> x;
    }
    segment_tree_t segment_tree(original);

    for (int32_t i = 0; i < q; i++) {
        int32_t op, l, r;
        cin >> op >> l >> r;
        l--;
        r--;
        if (op == 1) {
            int32_t v;
            cin >> v;
            segment_tree.update(l, r, v);
        }
        else if (op == query_op) {
            cout << segment_tree.query(l, r) << endl;
        }
        else {
            assert(false);
        }
    }

    return 0;
}
