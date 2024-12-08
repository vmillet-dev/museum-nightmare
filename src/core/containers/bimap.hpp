#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <unordered_map>
#include <concepts>
#include <stdexcept>
#include <utility>

namespace game {

template<typename Left, typename Right>
    requires std::movable<Left> && std::movable<Right> &&
             std::equality_comparable<Left> && std::equality_comparable<Right> &&
             std::default_initializable<Left> && std::default_initializable<Right>
class Bimap {
public:
    using value_type = std::pair<Right, Left>;

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<Right, Left>;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() = default;
        explicit const_iterator(typename std::unordered_map<Left, Right>::const_iterator it)
            : it_(it) {}

        reference operator*() const {
            pair_.first = it_->second;
            pair_.second = it_->first;
            return pair_;
        }

        pointer operator->() const {
            pair_.first = it_->second;
            pair_.second = it_->first;
            return &pair_;
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const { return it_ == other.it_; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }

    private:
        typename std::unordered_map<Left, Right>::const_iterator it_;
        mutable value_type pair_;
    };

private:
    std::unordered_map<Left, Right> left_to_right;
    std::unordered_map<Right, Left> right_to_left;

public:
    Bimap() = default;

    bool insert(const Left& left, const Right& right) {
        if (left_to_right.contains(left) || right_to_left.contains(right)) {
            return false;
        }
        left_to_right.emplace(left, right);
        right_to_left.emplace(right, left);
        return true;
    }

    bool insert(Left&& left, Right&& right) {
        if (left_to_right.contains(left) || right_to_left.contains(right)) {
            return false;
        }
        auto [l_it, l_inserted] = left_to_right.try_emplace(std::move(left), std::move(right));
        if (l_inserted) {
            right_to_left.try_emplace(l_it->second, l_it->first);
        }
        return l_inserted;
    }

    const Right& get_left(const Left& left) const {
        auto it = left_to_right.find(left);
        if (it == left_to_right.end()) {
            throw std::out_of_range("Key not found in left map");
        }
        return it->second;
    }

    const Left& get_right(const Right& right) const {
        auto it = right_to_left.find(right);
        if (it == right_to_left.end()) {
            throw std::out_of_range("Key not found in right map");
        }
        return it->second;
    }

    bool contains_left(const Left& left) const noexcept {
        return left_to_right.contains(left);
    }

    bool contains_right(const Right& right) const noexcept {
        return right_to_left.contains(right);
    }

    void clear() noexcept {
        left_to_right.clear();
        right_to_left.clear();
    }

    [[nodiscard]] size_t size() const noexcept {
        return left_to_right.size();
    }

    [[nodiscard]] bool empty() const noexcept {
        return left_to_right.empty();
    }

    const_iterator begin() const { return const_iterator(left_to_right.begin()); }
    const_iterator end() const { return const_iterator(left_to_right.end()); }
};

} // namespace game

#endif // BIMAP_HPP
