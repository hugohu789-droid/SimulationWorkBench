#include <gtest/gtest.h>
#include "../core/SimulationEngine.h"
#include "../core/SimulationConfig.h"

// Test configuration parameter validation logic
TEST(SimulationEngineTest, ConfigurationValidation) {
    SimulationEngine engine;
    SimulationConfig config;

    // Case 1, Normal configuration
    config.nx = 50;
    config.ny = 50;
    config.timeSteps = 100;
    config.diffusionCoeff = 0.1;
    EXPECT_NO_THROW(engine.setConfig(config));

    // Case 2, Illegal grid size
    config.nx = -5;
    EXPECT_THROW(engine.setConfig(config), std::invalid_argument);

    // Case 3, Illegal diffusion coefficient
    config.nx = 50;
    config.diffusionCoeff = -0.1;
    EXPECT_THROW(engine.setConfig(config), std::invalid_argument);
}

// Test simulation initialization logic
TEST(SimulationEngineTest, InitializationCheck) {
    SimulationEngine engine;
    SimulationConfig config;
    config.nx = 10;
    config.ny = 10;
    config.timeSteps = 1;
    engine.setConfig(config);

    // Run once (synchronization)
    SimulationResult result = engine.run();

    // Validation Result Dimensions
    EXPECT_EQ(result.nx(), 10);
    EXPECT_EQ(result.ny(), 10);

    // Verify whether the heat source diffuses at its center
    // (diffusion coefficient > 0, the value at the center should be
    // less than the initial 1.0, and the value around it should be greater than 0).
    // Center point (5, 5)
    double centerVal = result.value(5, 5);
    EXPECT_LT(centerVal, 1.0);
    EXPECT_GT(centerVal, 0.0);
}
