package @ZERON_ANDROID_PACKAGE_NAME@;

import org.libsdl.app.SDLActivity;


public class @ZERON_ANDROID_TARGET@Activity extends SDLActivity {

    @Override
    protected String getMainFunction() {
        return "main";
    }

    @Override
    protected String[] getLibraries() {
        return new String[]{"SDL2", "@ZERON_ANDROID_TARGET@"};
    }
}