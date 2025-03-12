#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

class MyTestEnvironment final : public testing::Environment
{
public:
    void SetUp() override
    {
#ifdef NDEBUG
        spdlog::set_level(spdlog::level::info);
#else
        spdlog::set_level(spdlog::level::debug);
#endif
    }
};

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    AddGlobalTestEnvironment(new MyTestEnvironment);
    return RUN_ALL_TESTS();
}