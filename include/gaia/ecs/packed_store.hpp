#pragma once

#include "gaia/ecs/entity.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace gaia {

template <typename TComponent>
class PackedStore {
public:
    [[nodiscard]] bool Contains(Entity entity) const { return Find(entity).has_value(); }
    [[nodiscard]] const TComponent* Get(Entity entity) const { const auto index = Find(entity); return index ? &m_Components[*index] : nullptr; }
    [[nodiscard]] TComponent* Get(Entity entity) { const auto index = Find(entity); return index ? &m_Components[*index] : nullptr; }

    bool InsertOrAssign(Entity entity, TComponent component) {
        const auto index = Find(entity);
        if (index) { m_Components[*index] = std::move(component); return false; }
        const auto position = std::lower_bound(m_Entities.begin(), m_Entities.end(), entity);
        const auto offset = static_cast<std::size_t>(position - m_Entities.begin());
        m_Entities.insert(position, entity);
        m_Components.insert(m_Components.begin() + static_cast<std::ptrdiff_t>(offset), std::move(component));
        return true;
    }

    bool Remove(Entity entity) {
        const auto index = Find(entity);
        if (!index) {
            return false;
        }
        m_Entities.erase(m_Entities.begin() + static_cast<std::ptrdiff_t>(*index));
        m_Components.erase(m_Components.begin() + static_cast<std::ptrdiff_t>(*index));
        return true;
    }
    [[nodiscard]] std::size_t Size() const { return m_Entities.size(); }

private:
    [[nodiscard]] std::optional<std::size_t> Find(Entity entity) const {
        const auto iterator = std::lower_bound(m_Entities.begin(), m_Entities.end(), entity);
        if (iterator == m_Entities.end() || *iterator != entity) return std::nullopt;
        return static_cast<std::size_t>(iterator - m_Entities.begin());
    }
    std::vector<Entity> m_Entities;
    std::vector<TComponent> m_Components;
};

} // namespace gaia
