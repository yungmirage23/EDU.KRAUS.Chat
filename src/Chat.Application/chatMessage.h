#pragma once
#include "pch.h"
#include <string>
#include <sstream>
#include "json.hpp"  // Для роботи з JSON


// Функція отримання поточного часу в форматі ISO 8601
inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    localtime_s(&tm_now, &time_t_now);  // Используем безопасную версию localtime

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%dT%H:%M:%S");  // Формат ISO 8601: YYYY-MM-DDTHH:MM:SS
    return oss.str();
}

// Структура для повідомлення чату
struct ChatMessage {
    std::string username;   // Ім'я користувача
    std::string timestamp;  // Час відправлення повідомлення
    std::string message;    // Текст повідомлення

    // Конструктор
    ChatMessage(const std::string& user, const std::string& msg)
        : username(user), message(msg) {
        timestamp = getCurrentTimestamp();  // Устанавливаем текущее время
    }

    // Метод для серіалізації структури в JSON
    nlohmann::json ToJson() const {
        return nlohmann::json{
            {"username", username},
            {"timestamp", timestamp},
            {"message", message}
        };
    }

    static ChatMessage FromJson(const nlohmann::json& jsonData) {
        ChatMessage msg(jsonData.at("username").get<std::string>(),  // Конструктор с параметрами
            jsonData.at("message").get<std::string>());
        msg.timestamp = jsonData.at("timestamp").get<std::string>();  // timestamp можно установить отдельно
        return msg;
    }
};