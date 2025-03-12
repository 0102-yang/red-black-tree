#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

class MyTestEnvironment final : public testing::Environment
{
public:
    void SetUp() override
    {
#if LOG_LEVEL == DEBUG
        spdlog::set_level(spdlog::level::debug);
#elif LOG_LEVEL == TRACE
        spdlog::set_level(spdlog::level::trace);
#else
        spdlog::set_level(spdlog::level::info);
#endif
    }
};

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    AddGlobalTestEnvironment(new MyTestEnvironment);
    return RUN_ALL_TESTS();
}