#pragma once

#include <functional>
#include <cstdint>

namespace dispatcher {

enum class TaskPriority : int8_t { High = -1, Normal = 0, Low = 1 };

using Task = std::function<void()>;

}  // namespace dispatcher
