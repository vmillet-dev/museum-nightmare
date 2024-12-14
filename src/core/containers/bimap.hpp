#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <unordered_map>
#include <concepts>
#include <stdexcept>
#include <utility>

namespace game {

template<typename Left, typename Right>
requires    std::movable<Left>&& std::movable<Right>&&
            std::equality_comparable<Left>&& std::equality_comparable<Right>&&
            std::default_initializable<Left>&& std::default_initializable<Right>
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
            : it_(it) {
        }
    
        value_type operator*() const {
            return { it_->second, it_->first };
        }
    
        pointer operator->() const {
            return &std::pair{ it_->second, it_->first };
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
    };
    
    Bimap(Bimap&& other) noexcept {
        left_to_right = std::move(other.left_to_right);
        right_to_left = std::move(other.right_to_left);
    }
    
    Bimap() = default;
    Bimap(const Bimap&) = delete;
    Bimap& operator=(const Bimap&) = delete;
    ~Bimap() = default;
    
    template <typename L, typename R>
    bool insert(L&& left, R&& right) {
        if (left_to_right.contains(left) || right_to_left.contains(right)) {
            return false;
        }
        left_to_right.emplace(left, right);
        right_to_left.emplace(right, left);
        return true;
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
    
    const_iterator begin() const {
        return const_iterator(left_to_right.begin());
    }
    
    const_iterator end() const {
        return const_iterator(left_to_right.end());
    }
    
private:
    std::unordered_map<Left, Right> left_to_right;
    std::unordered_map<Right, Left> right_to_left;
};

} // namespace game

#endif // BIMAP_HPP
