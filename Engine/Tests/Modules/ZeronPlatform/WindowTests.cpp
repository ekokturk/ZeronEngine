// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Platform/Window.h>
#include <Platform/WindowManager.h>

using namespace ::Zeron;

#if ZE_WINDOW_GLFW || ZE_WINDOW_WIN32

namespace ZeronPlatformTests
{
	class WindowTest : public ::testing::TestWithParam<WindowAPI> {
	  public:
		void SetUp() override
		{
			mWindowManager = std::make_unique<WindowManager>([](auto, auto) {});
			auto config = WindowConfig();
			config.mPreferredAPI = GetParam();
			mWindow = mWindowManager->RegisterWindow(config);
		}

		void TickFrame(int count = 1)
		{
			for (int i = 0; i < count; i++) {
				mWindowManager->Update();
			}
		}

		void TickForSeconds(float seconds = 1.f)
		{
			const auto start = std::chrono::high_resolution_clock::now();
			double elapsedTime = 0.0;
			do {
				const auto end = std::chrono::high_resolution_clock::now();
				elapsedTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
				TickFrame();
			}
			while (elapsedTime < (1 * 1000.0));
		}

	  protected:
		Window* mWindow = nullptr;
		std::unique_ptr<WindowManager> mWindowManager;
	};

	TEST_P(WindowTest, CreateRenamed_SetPrevName_HasPrevName)
	{
		const std::string namePrev = mWindow->GetName();
		const std::string nameNew = "New Name";
		mWindow->SetName(nameNew);
		TickFrame();
		ASSERT_EQ(nameNew, mWindow->GetName());

		mWindow->SetName(namePrev);
		TickFrame();

		EXPECT_EQ(namePrev, mWindow->GetName());
	}

	TEST_P(WindowTest, GetPosition_SetNewPosition_PrevAndCurrentPosChanged)
	{
		TickFrame();
		ASSERT_FALSE(mWindow->IsFullScreen());
		const Vec2i oldPos = mWindow->GetPos();

		const Vec2i newPos = { 54, 74 };
		mWindow->SetScreenPosition(newPos.X, newPos.Y);
		TickForSeconds(.5f);

		EXPECT_EQ(newPos, mWindow->GetPos());
		EXPECT_EQ(oldPos, mWindow->GetPosPrev());
	}

	TEST_P(WindowTest, CreateWindowed_SetSize_IsResized)
	{
		mWindow->SetFullScreen(false);
		TickForSeconds(.5f);
		ASSERT_FALSE(mWindow->IsFullScreen());

		const Vec2i oldSize = mWindow->GetSize();
		const Vec2i newSize(400, 500);
		mWindow->SetSize(newSize.X, newSize.Y);
		TickForSeconds(.5f);

		const Vec2i& currentSize = mWindow->GetSize();
		const Vec2i& currentSizePrev = mWindow->GetSizePrev();
		EXPECT_EQ(newSize, currentSize);
		EXPECT_EQ(oldSize, currentSizePrev);
	}

	TEST_P(WindowTest, CreateFullscreen_SetSizeAndRestore_NotResizedAndWindowed)
	{
		mWindow->SetFullScreen(true);
		TickFrame();
		ASSERT_TRUE(mWindow->IsFullScreen());
		const Vec2i fullScreenSize = mWindow->GetSize();
		const Vec2i fullScreenSizePrev = mWindow->GetSizePrev();

		mWindow->SetSize(400, 500);
		TickFrame();
		const Vec2i expectedSize = mWindow->GetSize();
		const Vec2i expectedSizePrev = mWindow->GetSizePrev();
		mWindow->SetFullScreen(false);
		TickFrame();

		EXPECT_EQ(expectedSize, fullScreenSize);
		EXPECT_EQ(expectedSizePrev, fullScreenSizePrev);
		EXPECT_FALSE(mWindow->IsFullScreen());
	}

	TEST_P(WindowTest, CreateFullscreen_SetPosition_IsNotRepositioned)
	{
		mWindow->SetFullScreen(true);
		TickFrame();
		ASSERT_TRUE(mWindow->IsFullScreen());
		const Vec2i fullScreenPos = mWindow->GetPos();
		const Vec2i fullScreenPosPrev = mWindow->GetPosPrev();

		mWindow->SetScreenPosition(200, 250);
		TickFrame();
		const Vec2i currentPos = mWindow->GetPos();
		const Vec2i currentPosPrev = mWindow->GetPosPrev();
		mWindow->SetFullScreen(false);
		TickFrame();

		EXPECT_EQ(fullScreenPos, currentPos);
		EXPECT_EQ(fullScreenPosPrev, currentPosPrev);
		EXPECT_FALSE(mWindow->IsFullScreen());
	}

	TEST_P(WindowTest, CreateMinimized_Restore_IsRestored)
	{
		mWindow->SetMinimized();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMinimized());

		mWindow->SetRestored();
		TickFrame();

		EXPECT_FALSE(mWindow->IsMinimized());
	}

	TEST_P(WindowTest, CreateMaximized_MinimizeAndRestore_IsMaximized)
	{
		mWindow->SetMaximized();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMaximized());

		mWindow->SetMinimized();
		TickFrame();
		mWindow->SetRestored();
		TickFrame();

		EXPECT_TRUE(mWindow->IsMaximized());
	}

	TEST_P(WindowTest, CreateMaximized_Restore_IsRestored)
	{
		mWindow->SetMaximized();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMaximized());

		mWindow->SetRestored();
		TickFrame();

		EXPECT_FALSE(mWindow->IsMaximized());
	}

	TEST_P(WindowTest, CreateMaximized_MinimizeAndRestoreToWindowed_HasInitialSize)
	{
		const Vec2i initialSize = mWindow->GetSize();
		mWindow->SetMaximized();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMaximized());
		const Vec2i maximizedSize = mWindow->GetSize();

		mWindow->SetMinimized();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMinimized());
		mWindow->SetRestored();
		TickFrame();
		ASSERT_TRUE(mWindow->IsMaximized());
		mWindow->SetRestored();
		TickFrame();

		const Vec2i& currentSize = mWindow->GetSize();
		const Vec2i& currentSizePrev = mWindow->GetSizePrev();
		EXPECT_EQ(initialSize, currentSize);
		EXPECT_EQ(maximizedSize, currentSizePrev);
	}

	TEST_P(WindowTest, CreateHidden_SetVisible_IsVisible)
	{
		mWindow->SetHidden();
		TickFrame();
		ASSERT_TRUE(mWindow->IsHidden());

		mWindow->SetVisible();
		TickFrame();

		EXPECT_FALSE(mWindow->IsHidden());
	}

#	if ZE_WINDOW_GLFW
	INSTANTIATE_TEST_SUITE_P(WindowAPI_GLFW, WindowTest, ::testing::Values(WindowAPI::GLFW));
#	endif
#	if ZE_WINDOW_WIN32
	INSTANTIATE_TEST_SUITE_P(WindowAPI_Win32, WindowTest, ::testing::Values(WindowAPI::Win32));
#	endif
}
#endif