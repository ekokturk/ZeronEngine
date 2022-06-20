// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Window/API/GLFW/WindowGLFW.h>
#include <Window/API/SDL/WindowSDL.h>
#include <Window/API/Win32/WindowWin32.h>

using namespace ::Zeron;

namespace TestModule_ZeronRenderer {
	
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

        static void TickForSeconds(float seconds = 1.f) {
            const auto start = std::chrono::high_resolution_clock::now();
		    double elapsedTime = 0.0;
            do {
                const auto end = std::chrono::high_resolution_clock::now();
                elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                TickFrame();
            }
            while (elapsedTime < (1 * 1000.0));
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
        const std::string namePrev = this->mWindow->GetName();
        const std::string nameNew = "New Name";
        this->mWindow->SetName(nameNew);
        this->TickFrame();
        ASSERT_EQ(nameNew, this->mWindow->GetName());

        this->mWindow->SetName(namePrev);
        this->TickFrame();

        EXPECT_EQ(namePrev, this->mWindow->GetName());
    }

    TYPED_TEST_P(WindowTest, GetPosition_SetNewPosition_PrevAndCurrentPosChanged)
    {
        ASSERT_FALSE(this->mWindow->IsFullScreen());
        const Vec2i oldPos = this->mWindow->GetPos();

        const Vec2i newPos = { 54, 74 };
        this->mWindow->SetScreenPosition(newPos.X, newPos.Y);
        this->TickForSeconds(.5f);

        EXPECT_EQ(newPos, this->mWindow->GetPos());
        EXPECT_EQ(oldPos, this->mWindow->GetPosPrev());
    }

    TYPED_TEST_P(WindowTest, CreateWindowed_SetSize_IsResized)
    {
        this->mWindow->SetFullScreen(false);
        this->TickForSeconds(.5f);
        ASSERT_FALSE(this->mWindow->IsFullScreen());

        const Vec2i oldSize = this->mWindow->GetSize();
        const Vec2i newSize(400, 500);
        this->mWindow->SetSize(newSize.X, newSize.Y);
        this->TickForSeconds(.5f);

        const Vec2i& currentSize = this->mWindow->GetSize();
        const Vec2i& currentSizePrev = this->mWindow->GetSizePrev();
        EXPECT_EQ(newSize, currentSize);
        EXPECT_EQ(oldSize, currentSizePrev);
    }

    TYPED_TEST_P(WindowTest, CreateFullscreen_SetSizeAndRestore_NotResizedAndWindowed)
    {
        this->mWindow->SetFullScreen(true);
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsFullScreen());
        const Vec2i fullScreenSize = this->mWindow->GetSize();
        const Vec2i fullScreenSizePrev = this->mWindow->GetSizePrev();

        this->mWindow->SetSize(400, 500);
        this->TickFrame();
        const Vec2i expectedSize = this->mWindow->GetSize();
        const Vec2i expectedSizePrev = this->mWindow->GetSizePrev();
        this->mWindow->SetFullScreen(false);
        this->TickFrame();

        EXPECT_EQ(expectedSize, fullScreenSize);
        EXPECT_EQ(expectedSizePrev, fullScreenSizePrev);
        EXPECT_FALSE(this->mWindow->IsFullScreen());
    }

    TYPED_TEST_P(WindowTest, CreateFullscreen_SetPosition_IsNotRepositioned)
    {
        this->mWindow->SetFullScreen(true);
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsFullScreen());
        const Vec2i fullScreenPos = this->mWindow->GetPos();
        const Vec2i fullScreenPosPrev = this->mWindow->GetPosPrev();

        this->mWindow->SetScreenPosition(200, 250);
        this->TickFrame();
        const Vec2i currentPos = this->mWindow->GetPos();
        const Vec2i currentPosPrev = this->mWindow->GetPosPrev();
        this->mWindow->SetFullScreen(false);
        this->TickFrame();

        EXPECT_EQ(fullScreenPos, currentPos);
        EXPECT_EQ(fullScreenPosPrev, currentPosPrev);
        EXPECT_FALSE(this->mWindow->IsFullScreen());
    }

    TYPED_TEST_P(WindowTest, CreateMinimized_Restore_IsRestored)
    {
        this->mWindow->SetMinimized();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMinimized());

        this->mWindow->SetRestored();
        this->TickFrame();

        EXPECT_FALSE(this->mWindow->IsMinimized());
    }

    TYPED_TEST_P(WindowTest, CreateMaximized_MinimizeAndRestore_IsMaximized)
    {
        this->mWindow->SetMaximized();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMaximized());

        this->mWindow->SetMinimized();
        this->TickFrame();
        this->mWindow->SetRestored();
        this->TickFrame();

        EXPECT_TRUE(this->mWindow->IsMaximized());
    }

    TYPED_TEST_P(WindowTest, CreateMaximized_Restore_IsRestored)
    {
        this->mWindow->SetMaximized();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMaximized());

        this->mWindow->SetRestored();
        this->TickFrame();

        EXPECT_FALSE(this->mWindow->IsMaximized());
    }

    TYPED_TEST_P(WindowTest, CreateMaximized_MinimizeAndRestoreToWindowed_HasInitialSize)
    {
        const Vec2i initialSize = this->mWindow->GetSize();
        this->mWindow->SetMaximized();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMaximized());
        const Vec2i maximizedSize = this->mWindow->GetSize();

        this->mWindow->SetMinimized();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMinimized());
        this->mWindow->SetRestored();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsMaximized());
        this->mWindow->SetRestored();
        this->TickFrame();

        const Vec2i& currentSize = this->mWindow->GetSize();
        const Vec2i& currentSizePrev = this->mWindow->GetSizePrev();
        EXPECT_EQ(initialSize, currentSize);
        EXPECT_EQ(maximizedSize, currentSizePrev);
    }
	
    TYPED_TEST_P(WindowTest, CreateHidden_SetVisible_IsVisible)
    {
        this->mWindow->SetHidden();
        this->TickFrame();
        ASSERT_TRUE(this->mWindow->IsHidden());

        this->mWindow->SetVisible();
        this->TickFrame();

        EXPECT_FALSE(this->mWindow->IsHidden());
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


#if ZE_WINDOW_GLFW
    INSTANTIATE_TYPED_TEST_SUITE_P(WindowAPI_GLFW, WindowTest, WindowGLFW);
#endif
#if ZE_WINDOW_SDL
    INSTANTIATE_TYPED_TEST_SUITE_P(WindowAPI_SDL, WindowTest, WindowSDL);
#endif
#if ZE_WINDOW_WIN32
    INSTANTIATE_TYPED_TEST_SUITE_P(WindowAPI_Win32, WindowTest, WindowWin32);
#endif
}



