#include "pch.h"
#include "stack.h"
#include <vector>
#include <string>

TEST(STACK, push) {
	stack<int> st;
	st.push(1);
	EXPECT_EQ(st.head(), 1);
	st.push(2);
	EXPECT_EQ(st.head(), 2);
	st.push(3);
	EXPECT_EQ(st.head(), 3);
}

TEST(STACK, pop) {
	stack<int> st;
	st.push(1);
	st.push(2);
	st.push(3);
	EXPECT_EQ(st.pop(), 3);
	EXPECT_EQ(st.head(), 2);
	EXPECT_EQ(st.pop(), 2);
	EXPECT_EQ(st.head(), 1);
}

stack<std::string> stringstack{};
TEST(StackFixture, PushMove)
{
	std::string string{ "I am a string" };
	stringstack.push(std::move(string));

	EXPECT_EQ(stringstack.head(), std::string{ "I am a string" });
}
