#pragma once
#include "pch.h"
#include <string>
#include <sstream>
#include "json.hpp"  // ��� ������ � JSON


// ������� ��������� ��������� ���� � ������ ISO 8601
inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    localtime_s(&tm_now, &time_t_now);  // ���������� ���������� ������ localtime

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%dT%H:%M:%S");  // ������ ISO 8601: YYYY-MM-DDTHH:MM:SS
    return oss.str();
}

// ��������� ��� ����������� ����
struct ChatMessage {
    std::string username;   // ��'� �����������
    std::string timestamp;  // ��� ����������� �����������
    std::string message;    // ����� �����������

    // �����������
    ChatMessage(const std::string& user, const std::string& msg)
        : username(user), message(msg) {
        timestamp = getCurrentTimestamp();  // ������������� ������� �����
    }

    // ����� ��� ���������� ��������� � JSON
    nlohmann::json ToJson() const {
        return nlohmann::json{
            {"username", username},
            {"timestamp", timestamp},
            {"message", message}
        };
    }

    static ChatMessage FromJson(const nlohmann::json& jsonData) {
        ChatMessage msg(jsonData.at("username").get<std::string>(),  // ����������� � �����������
            jsonData.at("message").get<std::string>());
        msg.timestamp = jsonData.at("timestamp").get<std::string>();  // timestamp ����� ���������� ��������
        return msg;
    }
};