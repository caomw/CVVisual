//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include "../src/util/util.hpp"

class ReferenceTest: public testing::Test{};

using cvv::util::Reference;
using cvv::util::makeRef;

TEST_F(ReferenceTest, Construction)
{
	int i = 3;
	Reference<int> ref1{i};
	EXPECT_EQ(*ref1, 3);
	Reference<int> ref2{&i};
	EXPECT_EQ(*ref2, 3);
}

TEST_F(ReferenceTest, Reassignment)
{
	int i1 = 3;
	int i2 = 4;
	Reference<int> ref{i1};
	EXPECT_EQ(ref.getPtr(), &i1);
	EXPECT_EQ(*ref, 3);
	ref = Reference<int>{i2};
	EXPECT_EQ(*ref, 4);
	EXPECT_EQ(ref.getPtr(), &i2);
}

TEST_F(ReferenceTest, Comparing)
{
	int i1 = 1, i2 = 1;
	auto ref1 = makeRef(i1);
	auto ref2 = makeRef(i1);
	auto ref3 = makeRef(i2);
	
	EXPECT_EQ(ref1 == ref2, true);
	EXPECT_EQ(ref1 != ref2, false);
	EXPECT_EQ(ref1 == ref3, false);
	EXPECT_EQ(ref1 != ref3, true);
}

TEST_F(ReferenceTest, MakeRef)
{
	int i1 = 3, i2 = 4;
	auto ref = makeRef(i1);
	EXPECT_EQ(*ref, 3);
	EXPECT_EQ(ref.getPtr(), &i1);
	ref = makeRef(i2);
	EXPECT_EQ(*ref, 4);
	EXPECT_EQ(ref.getPtr(), &i2);
}

TEST_F(ReferenceTest, ConstRefs)
{
	const int i = 3;
	auto ref1 = makeRef(i);
	Reference<const int> ref2{i};
	EXPECT_EQ(ref1, ref2);
}

TEST_F(ReferenceTest, ConstRefsFromMutable)
{
	int i;
	Reference<const int> ref{i};
	EXPECT_EQ(ref.getPtr(), &i);
}
