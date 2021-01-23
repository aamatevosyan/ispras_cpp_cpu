//
// Created by armen on 10/16/2020.
//

#include "gtest/gtest.h"
#include "lib/mstack.hpp"

unsigned long f(unsigned long x) {
    return x;
}

TEST(MTest, simple) {
    mstack<unsigned long> st(f, 10);
    EXPECT_FALSE(st.hashChanged());

    st.push(1);
    st.push(2);
    st.push(30);
    st.push(60);

    cout << st.dump() << endl;
    EXPECT_TRUE(st.hashChanged());
    EXPECT_EQ(4, st.size());
}

TEST(MTest, underflow) {
    mstack<unsigned long> st(f, 10);
    EXPECT_FALSE(st.hashChanged());

    EXPECT_ANY_THROW(st.pop());

    cout << st.dump() << endl;
    EXPECT_FALSE(st.hashChanged());
}

TEST(MTest, overflow) {
    mstack<unsigned long> st(f, 10);
    EXPECT_FALSE(st.hashChanged());

    st.push(1);
    st.push(2);
    st.push(30);
    st.push(60);
    st.push(1);
    st.push(2);
    st.push(30);
    st.push(60);
    st.push(50);
    st.push(40);
    EXPECT_ANY_THROW(st.push(1));
    EXPECT_EQ(10, st.size());

    cout << st.dump() << endl;
    EXPECT_TRUE(st.hashChanged());
}



