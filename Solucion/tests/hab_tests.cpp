#include "gtest-1.8.1/gtest.h"
#include "modelo/Habitacion.h"

TEST(Hab, constructor) {
    Habitacion h(10, set<Pos>{{0, 0}});
    EXPECT_EQ(h.tam(), 10);
    EXPECT_TRUE(h.ocupado({0,0}));
    EXPECT_FALSE(h.ocupado({0,1}));
    EXPECT_FALSE(h.ocupado({1,1}));
    EXPECT_FALSE(h.ocupado({1,0}));
}

TEST(Hab, ocupadas) {
    Habitacion h(2, set<Pos>{{0, 0}, {1, 1}});
    EXPECT_EQ(h.tam(), 2);
    EXPECT_TRUE(h.ocupado({0, 0}));
    EXPECT_TRUE(h.ocupado({1, 1}));
    EXPECT_FALSE(h.ocupado({1, 0}));
    EXPECT_FALSE(h.ocupado({0, 1}));
}
