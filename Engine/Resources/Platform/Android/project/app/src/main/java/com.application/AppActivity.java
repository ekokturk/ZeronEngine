package @ZERON_ANDROID_PACKAGE_NAME@;

import com.zeron.engine.ZeronActivity;

public class @ZERON_ANDROID_TARGET@Activity extends ZeronActivity {

    static {
        System.loadLibrary("@ZERON_ANDROID_TARGET@");
    }
}