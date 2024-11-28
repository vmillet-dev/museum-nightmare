#include <gtest/gtest.h>
#include "game/objects/ParallaxLayer.hpp"

TEST(ParallaxLayerTest, BasicParallaxFactorTest) {
    game::ParallaxLayer layer(1.0f);
    EXPECT_FLOAT_EQ(layer.getParallaxFactor(), 1.0f);
}
