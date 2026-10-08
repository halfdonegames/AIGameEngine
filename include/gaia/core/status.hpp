#pragma once

#include <string>
#include <utility>

namespace gaia {

enum class ErrorCode { kInvalidArgument, kNotFound, kAlreadyExists, kParseError, kSchemaError };

struct Status {
    ErrorCode code{};
    std::string message;
    [[nodiscard]] static Status Ok() { return {}; }
    [[nodiscard]] static Status Error(ErrorCode code, std::string message) {
        return Status{code, std::move(message)};
    }
    [[nodiscard]] bool ok() const { return message.empty(); }
};

template <typename T>
struct Result {
    T value{};
    Status status{};
    [[nodiscard]] bool ok() const { return status.ok(); }
    [[nodiscard]] static Result Success(T value) { return Result{std::move(value), Status::Ok()}; }
    [[nodiscard]] static Result Failure(ErrorCode code, std::string message) {
        return Result{T{}, Status::Error(code, std::move(message))};
    }
};

}  // namespace gaia
