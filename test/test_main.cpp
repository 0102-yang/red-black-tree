#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

class MyTestEnvironment final : public testing::Environment {
public:
    void SetUp() override {
        if (const std::string log_level = std::getenv("LOG_LEVEL"); log_level == "DEBUG") {
            spdlog::set_level(spdlog::level::debug);
        } else if (log_level == "INFO") {
            spdlog::set_level(spdlog::level::info);
        } else {
            spdlog::set_level(spdlog::level::warn);
        }
    }
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    AddGlobalTestEnvironment(new MyTestEnvironment);
    return RUN_ALL_TESTS();
}