// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "ZeronTestFramework.h"
#include "Window/Platform/Win32/WindowWin32.h"
#include "Window/Platform/SDL/WindowSDL.h"
#include "Window/Platform/GLFW/WindowGLFW.h"

using namespace ::Zeron;

namespace ZeronRendererTests {
	
    template <typename T>
    class WindowTest : public ::testing::Test {
    public:
        static void SetUpTestSuite() {
            mWindow = std::make_unique<T>(WindowConfig());
            mWindow->Init();
        }

        static void TickFrame(int count = 1) {
        	for(int i = 0; i < count; i++) {
                mWindow->BeginFrame();
                mWindow->EndFrame();
        	}
        }

        static void TearDownTestSuite() {
            mWindow = nullptr;
        }

    protected:
        static std::unique_ptr<Window> mWindow;
    };

    template<typename T>
    std::unique_ptr<Window> WindowTest<T>::mWindow = nullptr;

    TYPED_TEST_SUITE_P(WindowTest);

    TYPED_TEST_P(WindowTest, CreateRenamed_SetPrevName_HasPrevName)
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

    TYPED_TEST_P(WindowTest, GetPosition_SetNewPosition_PrevAndCurrentPosChanged)
    {
        ASSERT_FALSE(mWindow->IsFullScreen());
        const Vec2i oldPos = mWindow->GetPos();

        const Vec2i newPos = { 200, 250 };
        mWindow->SetScreenPosition(newPos.X, newPos.Y);
        TickFrame();

        EXPECT_EQ(newPos, mWindow->GetPos());
        EXPECT_EQ(oldPos, mWindow->GetPosPrev());
    }

    TYPED_TEST_P(WindowTest, CreateWindowed_SetSize_IsResized)
    {
        mWindow->SetFullScreen(false);
        TickFrame();
        ASSERT_FALSE(mWindow->IsFullScreen());

        const Vec2i oldSize = mWindow->GetSize();
        const Vec2i newSize(400, 500);
        mWindow->SetSize(newSize.X, newSize.Y);
        TickFrame();

        const Vec2i& currentSize = mWindow->GetSize();
        const Vec2i& currentSizePrev = mWindow->GetSizePrev();
        EXPECT_EQ(newSize, currentSize);
        EXPECT_EQ(oldSize, currentSizePrev);
    }

    TYPED_TEST_P(WindowTest, CreateFullscreen_SetSizeAndRestore_NotResizedAndWindowed)
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

    TYPED_TEST_P(WindowTest, CreateFullscreen_SetPosition_IsNotRepositioned)
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

    TYPED_TEST_P(WindowTest, CreateMinimized_Restore_IsRestored)
    {
        mWindow->SetMinimized();
        TickFrame();
        ASSERT_TRUE(mWindow->IsMinimized());

        mWindow->SetRestored();
        TickFrame();

        EXPECT_FALSE(mWindow->IsMinimized());
    }

    TYPED_TEST_P(WindowTest, CreateMaximized_MinimizeAndRestore_IsMaximized)
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

    TYPED_TEST_P(WindowTest, CreateMaximized_Restore_IsRestored)
    {
        mWindow->SetMaximized();
        TickFrame();
        ASSERT_TRUE(mWindow->IsMaximized());

        mWindow->SetRestored();
        TickFrame();

        EXPECT_FALSE(mWindow->IsMaximized());
    }

    TYPED_TEST_P(WindowTest, CreateMaximized_MinimizeAndRestoreToWindowed_HasInitialSize)
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
	
    TYPED_TEST_P(WindowTest, CreateHidden_SetVisible_IsVisible)
    {
        mWindow->SetHidden();
        TickFrame();
        ASSERT_TRUE(mWindow->IsHidden());

        mWindow->SetVisible();
        TickFrame();

        EXPECT_FALSE(mWindow->IsHidden());
    }
	

    REGISTER_TYPED_TEST_SUITE_P(WindowTest,
        // Spec
        CreateRenamed_SetPrevName_HasPrevName,
        // Position
        GetPosition_SetNewPosition_PrevAndCurrentPosChanged,
        CreateFullscreen_SetPosition_IsNotRepositioned,
        // Size
        CreateWindowed_SetSize_IsResized,
        CreateMaximized_MinimizeAndRestoreToWindowed_HasInitialSize,
        CreateFullscreen_SetSizeAndRestore_NotResizedAndWindowed,
        // State
        CreateMinimized_Restore_IsRestored,
        CreateMaximized_MinimizeAndRestore_IsMaximized,
        CreateMaximized_Restore_IsRestored,
        CreateHidden_SetVisible_IsVisible
    );


    using WINDOW_TYPES = ::testing::Types<
        WindowGLFW,
		WindowSDL,
		WindowWin32
	>;
    INSTANTIATE_TYPED_TEST_SUITE_P(API, WindowTest, WINDOW_TYPES);

}



