package com.example.iot_kipas;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;


public class MainActivity extends AppCompatActivity {

    String suhu;
    String pembatas;

    TextView tampil_suhu;
    EditText tampil_pembatas;
    Button ClickPembatas;
    ToggleButton toggle1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_menu);

        toggle1 = (ToggleButton) findViewById(R.id.toggle1);
        tampil_suhu = (TextView) findViewById(R.id.tampil_suhu);
        tampil_pembatas = (EditText) findViewById(R.id.pembatasnya);
        ClickPembatas = (Button) findViewById(R.id.ClickPembatas);

        DatabaseReference dref = FirebaseDatabase.getInstance().getReference();
        dref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                suhu = snapshot.child("/miniproject3/suhu").getValue().toString();
                tampil_suhu.setText(suhu);

                pembatas = snapshot.child("/miniproject3/pembatas").getValue().toString();
                tampil_pembatas.setText(pembatas);

            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        ClickPembatas.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DatabaseReference myRef = FirebaseDatabase.getInstance().getReference("/miniproject3/pembatas");
                String temp = tampil_pembatas.getText().toString();
                int value = 0;
                if(!"".equals(temp)) {
                    value = Integer.parseInt(temp);
                }
                myRef.setValue(value);
            }
        });

        toggle1.setOnCheckedChangeListener((buttonView, isChecked) ->  {
            if(isChecked) {
                DatabaseReference on_off = FirebaseDatabase.getInstance().getReference("/miniproject3/on_off");
                on_off.setValue(1);

            }
            else {
                DatabaseReference on_off = FirebaseDatabase.getInstance().getReference("/miniproject3/on_off");
                on_off.setValue(0);
            }
        });
    }

}
