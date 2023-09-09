package com.zeron.engine;

import android.os.Bundle;
import android.view.View;
import com.google.androidgamesdk.GameActivity;

public class ZeronActivity extends GameActivity {

    @Override
    protected void onCreate(Bundle instance) {
        super.onCreate(instance);

        // Enforce full screen
        final int flags = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
        getWindow().getDecorView().setSystemUiVisibility(flags);
    }

}