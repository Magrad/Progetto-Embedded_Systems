package unibo.btclient;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Gravity;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.util.UUID;

import unibo.btlib.BluetoothChannel;
import unibo.btlib.BluetoothUtils;
import unibo.btlib.ConnectToBluetoothServerTask;
import unibo.btlib.ConnectionTask;
import unibo.btlib.RealBluetoothChannel;
import unibo.btlib.exceptions.BluetoothDeviceNotFound;
import unibo.btclient.utils.C;

public class MainActivity extends AppCompatActivity {
    private BluetoothChannel btChannel;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        if(btAdapter != null && !btAdapter.isEnabled()) {
            startActivityForResult(
                new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                C.bluetooth.ENABLE_BT_REQUEST
            );
        }

        initUI();
    }

    private void initUI() {
        connectionEstablished(false);

        findViewById(R.id.manualControl_btn).setOnClickListener(l -> {
            l.setEnabled(false);
            try {
                connectToBTServer();
            } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                Toast toast = Toast.makeText(this, "Bluetooth device not found !", Toast.LENGTH_LONG);
                toast.setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL, 0, 0);
                toast.show();
                bluetoothDeviceNotFound.printStackTrace();
            } finally {
                l.setEnabled(true);
            }
        });
    }

    @Override
    protected void onStop() {
        btChannel.sendMessage(C.bluetooth.codes.BT_CLOSED);

        super.onStop();

        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode,
                                    @Nullable final Intent data) {
        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK) {
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED) {
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    private void connectToBTServer() throws BluetoothDeviceNotFound {

        final BluetoothDevice serverDevice = BluetoothUtils
                .getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {
                Button btn = findViewById(R.id.manualControl_btn);

                // Disable the connection button and display that is currently connected
                btn.setEnabled(false);
                btn.setText(String.format(
                        "Connected : %s",
                        serverDevice.getName()
                ));

                btChannel = channel;

                // Once gained access to the bluetooth, we are able to send and receive messages
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        receivedMessage = receivedMessage.trim();
                        Log.d("RECEIVED", receivedMessage);

                        /* Every time that the bluetooth receives a message it goes
                           through a series of checks to determine which action is
                           necessary */

                        if(receivedMessage.equals(C.bluetooth.codes.BT_CONNECTION_ERR)) {
                            btn.setEnabled(true);
                            btn.setText(C.bluetooth.MANUAL_TEXT_BTN);
                        }

                        // Once the "handshake" has been accomplished the UI is turned on
                        if(receivedMessage.equals(C.bluetooth.codes.BT_CONNECTION_OK)) {
                            connectionEstablished(true);
                        }

                        /* The following sets of statements checks whenever a LED or
                           motor button has been clicked in order to change theirs
                           state and color to better acknowledge a user input */
                        if(receivedMessage.equals(C.bluetooth.codes.BT_LED_0_ON)) {
                            Button btn = findViewById(R.id.led_0_btn);
                            btn.setTextColor(Color.GREEN);
                        }

                        if(receivedMessage.equals(C.bluetooth.codes.BT_LED_0_OFF)) {
                            Button btn = findViewById(R.id.led_0_btn);
                            btn.setTextColor(Color.RED);
                        }

                        if(receivedMessage.equals(C.bluetooth.codes.BT_LED_1_ON)) {
                            Button btn = findViewById(R.id.led_1_btn);
                            btn.setTextColor(Color.GREEN);
                        }

                        if(receivedMessage.equals(C.bluetooth.codes.BT_LED_1_OFF)) {
                            Button btn = findViewById(R.id.led_1_btn);
                            btn.setTextColor(Color.RED);
                        }

                        if(receivedMessage.contains(C.bluetooth.codes.BT_LED_2)) {
                            TextView txt = findViewById(R.id.led_2_brightness_txt);
                            receivedMessage = receivedMessage.replaceAll(C.bluetooth.codes.BT_LED_2, "");
                            receivedMessage = receivedMessage.replace("_",  "");
                            txt.setText(receivedMessage);


                            if(Integer.parseInt(txt.getText().toString()) == C.LED_INTERVAL) {
                                findViewById(R.id.led_2_brightness_up_btn).setEnabled(false);
                            } else if(Integer.parseInt(txt.getText().toString()) == 0) {
                                findViewById(R.id.led_2_brightness_down_btn).setEnabled(false);
                            } else {
                                findViewById(R.id.led_2_brightness_up_btn).setEnabled(true);
                                findViewById(R.id.led_2_brightness_down_btn).setEnabled(true);
                            }

                        }

                        if(receivedMessage.contains(C.bluetooth.codes.BT_LED_3)) {
                            TextView txt = findViewById(R.id.led_3_brightness_txt);
                            receivedMessage = receivedMessage.replaceAll(C.bluetooth.codes.BT_LED_3, "");
                            receivedMessage = receivedMessage.replace("_",  "");
                            txt.setText(receivedMessage);


                            if(Integer.parseInt(txt.getText().toString()) == C.LED_INTERVAL) {
                                findViewById(R.id.led_3_brightness_up_btn).setEnabled(false);
                            } else if(Integer.parseInt(txt.getText().toString()) == 0) {
                                findViewById(R.id.led_3_brightness_down_btn).setEnabled(false);
                            } else {
                                findViewById(R.id.led_3_brightness_up_btn).setEnabled(true);
                                findViewById(R.id.led_3_brightness_down_btn).setEnabled(true);
                            }
                        }

                        if(receivedMessage.contains(C.bluetooth.codes.SM_SPEED)) {
                            TextView txt = findViewById(R.id.irrigation_txt);
                            txt.setText(receivedMessage.substring(9, 10));


                            if(Integer.parseInt(txt.getText().toString()) == C.SERVO_INTERVAL) {
                                findViewById(R.id.irrigation_up_btn).setEnabled(false);
                            } else if(Integer.parseInt(txt.getText().toString()) == 0) {
                                findViewById(R.id.irrigation_down_btn).setEnabled(false);
                            } else {
                                findViewById(R.id.irrigation_up_btn).setEnabled(true);
                                findViewById(R.id.irrigation_down_btn).setEnabled(true);
                            }

                            if(receivedMessage.contains(C.bluetooth.codes.SM_ON)) {
                                Button btn = findViewById(R.id.irrigation_OnOff_btn);
                                btn.setTextColor(Color.GREEN);
                            }

                            if(receivedMessage.contains(C.bluetooth.codes.SM_OFF)) {
                                Button btn = findViewById(R.id.irrigation_OnOff_btn);
                                btn.setTextColor(Color.RED);
                            }
                        }

                        /* Once the systems goes in ALARM Mode the ALARM button changes
                           its state and color until an user acknowledge the system
                           malfunction */
                        if(receivedMessage.contains(C.bluetooth.codes.BT_ALARM)) {
                            ImageButton iBtn = findViewById(R.id.alarmBtn);
                            iBtn.setColorFilter(Color.RED);
                        }

                        if(receivedMessage.contains(C.bluetooth.codes.BT_NOMINAL)) {
                            ImageButton iBtn = findViewById(R.id.alarmBtn);
                            iBtn.setColorFilter(Color.parseColor("#4CAF50"));
                        }

                        if(receivedMessage.contains(C.bluetooth.BT_MSG + C.bluetooth.codes.SETUP + ":" + C.bluetooth.codes.DONE)) {
                            channel.sendMessage(C.bluetooth.BT_MSG + C.bluetooth.codes.DONE);
                        }

                        if(receivedMessage.contains(C.bluetooth.codes.SERIAL_CLOSED)) {
                            onConnectionCanceled();
                        }
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
                    }
                });
                Log.d("SENDING: ", C.bluetooth.codes.BT_CONNECTION_REQUEST);
                btChannel.sendMessage(C.bluetooth.codes.BT_CONNECTION_REQUEST);
            }

            @Override
            public void onConnectionCanceled() {
                /* If the connection is closed the control goes back to
                   the service app by sending a closing message
                 */
                if(btChannel != null) {
                    btChannel.sendMessage("AUTO");
                }

                Button btn = findViewById(R.id.manualControl_btn);
                btn.setText(C.bluetooth.MANUAL_TEXT_BTN);
                btn.setEnabled(true);

                connectionEstablished(false);
            }
        }).execute();
    }

    /**
     * Function used to activate and deactivate the settings
     * buttons. While the app has not gained control over the
     * service app, all of the buttons will not have any event
     * attached to them, and they will appear disabled. Once
     * the operator has gain control, all of the buttons will
     * be enabled and get their respective on click events
     * @param state current display state
     */
    private void connectionEstablished(boolean state) {

        // Cycles through each component's button
        for(int btnID : C.btns) {
            Button btn = findViewById(btnID);
            btn.setEnabled(state);

            /* Attaches to each button their respective listeners
               and enables them in order for the user to press them */
            if(state) {
                btn.setOnClickListener(l -> {
                    String tag = btn.getTag().toString();
                    String sub = tag;

                    if(tag.length() >= 5) {
                        sub = tag.substring(0, 5);
                    }

                    String operator = "";

                    if(tag.contains("up")) {
                        operator = "+";
                    }
                    if(tag.contains("down")) {
                        operator = "-";
                    }

                    String msg = sub + operator;

                    btChannel.sendMessage(msg.toUpperCase());
                    Log.d("BTN_CLICK", msg.toUpperCase());
                });
                btn.setEnabled(true);
                btn.setTextColor(Color.BLACK);
                btn.setBackgroundTintList(ColorStateList.valueOf(0xFF2196F3));
            } else {
                /* Turns off the interface once the connection is closed so
                   no more changes can be made to the system */
                btn.setEnabled(false);
                btn.setOnClickListener(null);
                btn.setTextColor(Color.GRAY);
                btn.setBackgroundTintList(ColorStateList.valueOf(0x1A000000));
                for(int txtID : C.txtViews) {
                    TextView txt = findViewById(txtID);
                    txt.setText("0");
                }
            }
        }

        // Repeats the same procedure for the alarm button as well
        ImageButton iBtn = findViewById(R.id.alarmBtn);

        if(state) {
            iBtn.setOnClickListener(l -> {
                String tag = iBtn.getTag().toString();

                btChannel.sendMessage(tag.toUpperCase() + ":OFF");
                Log.d("BTN_CLICK", tag.toUpperCase() + ":OFF");
            });
            iBtn.setEnabled(true);
        } else {
            iBtn.setEnabled(false);
            iBtn.setOnClickListener(null);
            iBtn.setColorFilter(Color.parseColor("#4CAF50"));
        }
    }
}
