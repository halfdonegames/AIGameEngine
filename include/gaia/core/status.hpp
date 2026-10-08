#pragma once

#include <string>
#include <utility>

namespace gaia {

class Status {
public:
    static Status Ok() { return Status(true, {}); }
    static Status Error(std::string message) { return Status(false, std::move(message)); }

    [[nodiscard]] bool IsOk() const { return m_IsOk; }
    [[nodiscard]] const std::string& Message() const { return m_Message; }

private:
    Status(bool isOk, std::string message) : m_IsOk(isOk), m_Message(std::move(message)) {}

    bool m_IsOk;
    std::string m_Message;
};

template <typename TValue>
class Result {
public:
    static Result Success(TValue value) { return Result(Status::Ok(), std::move(value)); }
    static Result Failure(std::string message) { return Result(Status::Error(std::move(message)), {}); }

    [[nodiscard]] bool IsOk() const { return m_Status.IsOk(); }
    [[nodiscard]] const Status& GetStatus() const { return m_Status; }
    [[nodiscard]] const TValue& Value() const { return m_Value; }
    [[nodiscard]] TValue& Value() { return m_Value; }

private:
    Result(Status status, TValue value) : m_Status(std::move(status)), m_Value(std::move(value)) {}

    Status m_Status;
    TValue m_Value {};
};

} // namespace gaia
