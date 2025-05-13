// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Net/WebSocket/WebSocketFrame.h>

using namespace ::Zeron;
using namespace ::Zeron::Net;

namespace TestModule_ZeronNet
{
	TEST(WebSocketFrameTests, DefaultTextFrame_SerializeThenDeserialize_HasSameFrame)
	{
		WebSocketFrame expected;
		std::string payload("My text payload");
		const WebSocketFrame initial(WebSocketFrame::OpCode::Text, ByteBuffer::Create(std::span{ payload }));
		ASSERT_TRUE(initial.IsDataFrame());
		ASSERT_TRUE(initial.IsFinalFrame());
		ASSERT_TRUE(initial.HasMask());

		const ByteBuffer serialized = initial.Serialize();
		const auto readBytes = expected.Deserialize(serialized.GetView());
		ASSERT_TRUE(readBytes.has_value());
		ASSERT_EQ(serialized.size(), *readBytes);

		ASSERT_EQ(expected, initial);
	}

	TEST(WebSocketFrameTests, UnmaskedTextFrame_SerializeThenDeserialize_HasSameFrame)
	{
		WebSocketFrame expected;
		std::string payload("My text payload");
		const WebSocketFrame initial(WebSocketFrame::OpCode::Text, ByteBuffer::Create(std::span{ payload }), true, false);
		ASSERT_TRUE(initial.IsDataFrame());
		ASSERT_TRUE(initial.IsFinalFrame());
		ASSERT_FALSE(initial.HasMask());

		const ByteBuffer serialized = initial.Serialize();
		const auto readBytes = expected.Deserialize(serialized.GetView());
		ASSERT_TRUE(readBytes.has_value());
		ASSERT_EQ(serialized.size(), *readBytes);

		ASSERT_EQ(expected, initial);
	}

	TEST(WebSocketFrameTests, FragmentedTextFrame_SerializeThenDeserialize_HasSameFrame)
	{
		const std::string expectedPayload(100000, 'A');

		const std::vector<WebSocketFrame> fragments = WebSocketFrame::CreateFragments(expectedPayload, 10000);
		ASSERT_EQ(10, fragments.size());
		ASSERT_TRUE(fragments[0].IsDataFrame());
		for (int i = 1; i < 9; ++i) {
			ASSERT_TRUE(fragments[i].IsContinuationFrame());
		}
		ASSERT_TRUE(fragments[9].IsFinalFrame());
		const auto result = WebSocketFrame::CreatePayloadFromFragments(fragments);
		ASSERT_FALSE(result.HasError());

		const bool isSame = result.Value().IsEqual(std::span{ expectedPayload });
		ASSERT_TRUE(isSame);
	}

	TEST(WebSocketFrameTests, UnmaskedFragmentedTextFrame_SerializeThenDeserialize_HasSameFrame)
	{
		const std::string expectedPayload(100001, 'B');

		const std::vector<WebSocketFrame> fragments = WebSocketFrame::CreateFragments(expectedPayload, 10000, false);
		ASSERT_EQ(11, fragments.size());
		ASSERT_TRUE(fragments[0].IsDataFrame());
		for (int i = 1; i < 10; ++i) {
			ASSERT_TRUE(fragments[i].IsContinuationFrame());
		}
		ASSERT_TRUE(fragments[10].IsFinalFrame());
		const auto result = WebSocketFrame::CreatePayloadFromFragments(fragments);
		ASSERT_FALSE(result.HasError());

		const bool isSame = result.Value().IsEqual(std::span{ expectedPayload });
		ASSERT_TRUE(isSame);
	}

	TEST(WebSocketFrameTests, FragmentedSingleBinaryFrame_SerializeThenDeserialize_HasSameFrame)
	{
		const ByteBuffer expectedPayload(10, static_cast<std::byte>(20));

		const std::vector<WebSocketFrame> fragments = WebSocketFrame::CreateFragments(expectedPayload, 10000, false);
		ASSERT_EQ(1, fragments.size());
		ASSERT_TRUE(fragments[0].IsDataFrame());
		ASSERT_TRUE(fragments[0].IsFinalFrame());
		const auto result = WebSocketFrame::CreatePayloadFromFragments(fragments);
		ASSERT_FALSE(result.HasError());

		const bool isSame = result.Value().IsEqual(std::span{ expectedPayload });
		ASSERT_TRUE(isSame);
	}
}