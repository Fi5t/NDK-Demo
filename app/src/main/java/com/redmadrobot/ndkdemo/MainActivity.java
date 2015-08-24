package com.redmadrobot.ndkdemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("security");
    }

    public native String encrypt(String plainText);
    public native String decrypt(String ciphertext);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final EditText inputText = (EditText) findViewById(R.id.input_text);
        final Button encryptButton = (Button) findViewById(R.id.encrypt_button);
        final TextView cipherText = (TextView) findViewById(R.id.cipher_text);
        final TextView plainText = (TextView) findViewById(R.id.plain_text);

        encryptButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View v) {
                String cipher = encrypt(inputText.getText().toString());

                cipherText.setText(cipher);
                plainText.setText(decrypt(cipher));
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
