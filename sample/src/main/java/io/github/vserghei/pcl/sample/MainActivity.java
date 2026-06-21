package io.github.vserghei.pcl.sample;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView textView = new TextView(this);
        textView.setPadding(48, 48, 48, 48);
        textView.setTextSize(18);
        textView.setText(runPclSmokeTest());
        setContentView(textView);
    }

    private native String runPclSmokeTest();
}
