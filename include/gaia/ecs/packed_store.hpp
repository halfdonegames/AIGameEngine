#pragma once
#include <cstddef>
#include <optional>
#include <vector>
#include "gaia/ecs/entity.hpp"

namespace gaia {
template <typename T>
class PackedStore {
  public:
    bool contains(Entity entity) const {
        return entity.index < sparse_.size() && sparse_[entity.index] != kMissing;
    }
    T* get(Entity entity) {
        if (!contains(entity)) return nullptr;
        return &data_[sparse_[entity.index]];
    }
    const T* get(Entity entity) const {
        if (!contains(entity)) return nullptr;
        return &data_[sparse_[entity.index]];
    }
    bool insert(Entity entity, T value) {
        if (contains(entity)) return false;
        if (entity.index >= sparse_.size()) sparse_.resize(entity.index + 1, kMissing);
        sparse_[entity.index] = data_.size();
        entities_.push_back(entity);
        data_.push_back(std::move(value));
        return true;
    }
    bool erase(Entity entity) {
        if (!contains(entity)) return false;
        const size_t removed = sparse_[entity.index];
        const size_t last = data_.size() - 1;
        if (removed != last) {
            data_[removed] = std::move(data_[last]);
            entities_[removed] = entities_[last];
            sparse_[entities_[removed].index] = removed;
        }
        data_.pop_back(); entities_.pop_back(); sparse_[entity.index] = kMissing;
        return true;
    }
    void clear() { data_.clear(); entities_.clear(); sparse_.clear(); }
    [[nodiscard]] size_t size() const { return data_.size(); }
    [[nodiscard]] const std::vector<Entity>& entities() const { return entities_; }
  private:
    static constexpr size_t kMissing = static_cast<size_t>(-1);
    std::vector<T> data_; std::vector<Entity> entities_; std::vector<size_t> sparse_;
};
}  // namespace gaia
