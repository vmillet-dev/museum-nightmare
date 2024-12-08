#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <unordered_map>
#include <concepts>
#include <stdexcept>

namespace cpp23 {

template<typename Left, typename Right>
    requires std::movable<Left> && std::movable<Right> &&
             std::equality_comparable<Left> && std::equality_comparable<Right> &&
             std::default_initializable<Left> && std::default_initializable<Right>
class bimap {
private:
    std::unordered_map<Left, Right> left_to_right;
    std::unordered_map<Right, Left> right_to_left;

public:
    bimap() = default;

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

        // Store moved values
        auto [l_it, l_inserted] = left_to_right.try_emplace(std::move(left), std::move(right));
        if (l_inserted) {
            right_to_left.try_emplace(l_it->second, l_it->first);
        }
        return l_inserted;
    }

    [[nodiscard]] const Right* get_right(const Left& left) const noexcept {
        auto it = left_to_right.find(left);
        return it != left_to_right.end() ? &it->second : nullptr;
    }

    [[nodiscard]] const Left* get_left(const Right& right) const noexcept {
        auto it = right_to_left.find(right);
        return it != right_to_left.end() ? &it->second : nullptr;
    }

    bool remove(const Left& left) {
        auto it = left_to_right.find(left);
        if (it == left_to_right.end()) {
            return false;
        }
        right_to_left.erase(it->second);
        left_to_right.erase(it);
        return true;
    }

    bool remove_by_right(const Right& right) {
        auto it = right_to_left.find(right);
        if (it == right_to_left.end()) {
            return false;
        }
        left_to_right.erase(it->second);
        right_to_left.erase(it);
        return true;
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
};

} // namespace cpp23

#endif // BIMAP_HPP
