package unibo.btclient.utils;

import unibo.btclient.R;

public class C {

    public static final String APP_LOG_TAG = "BT CLN";
    public static final int LED_INTERVAL = 4;
    public static final int SERVO_INTERVAL = 4;

    public static final int btns[] = {
            R.id.led_0_btn, R.id.led_1_btn,
            R.id.led_2_brightness_down_btn, R.id.led_2_brightness_up_btn,
            R.id.led_3_brightness_down_btn, R.id.led_3_brightness_up_btn,
            R.id.irrigation_OnOff_btn, R.id.irrigation_down_btn, R.id.irrigation_up_btn
    };

    public static final int txtViews[] = {
            R.id.led_2_brightness_txt,
            R.id.led_3_brightness_txt,
            R.id.irrigation_txt
    };

    public static class bluetooth {
        public static final int ENABLE_BT_REQUEST = 1;
        // Replace this string with that of the device acting as server
        public static final String BT_DEVICE_ACTING_AS_SERVER_NAME = "SmartGardenBT";
        public static final String BT_SERVER_UUID = "7ba55836-01eb-11e9-8eb2-f2801f1b9fd1";
        public static final String MANUAL_TEXT_BTN = "Require Manual Control";
        public static final String BT_MSG = "BT:";

        public static class codes {
            public static final String BT_CONNECTION_OK = BT_MSG + "OK";
            public static final String BT_CONNECTION_ERR = BT_MSG + "ERR";
            public static final String BT_CONNECTION_REQUEST = BT_MSG + "MANUAL";
            public static final String BT_CLOSED = BT_MSG + "AUTO";
            public static final String BT_ALARM = BT_MSG + "ALARM";
            public static final String BT_NOMINAL = BT_MSG + "NOMINAL";
            public static final String SERIAL_CLOSED = BT_MSG + "CC";
            public static final String SETUP  = "SETUP";
            public static final String DONE = "DONE";
            public static final String BT_LED_0_ON = BT_MSG + "LED_0_1";
            public static final String BT_LED_0_OFF = BT_MSG + "LED_0_0";
            public static final String BT_LED_1_ON = BT_MSG + "LED_1_1";
            public static final String BT_LED_1_OFF = BT_MSG + "LED_1_0";
            public static final String BT_LED_2 = BT_MSG + "LED_2";
            public static final String BT_LED_3 = BT_MSG + "LED_3";
            public static final String SM_ON = BT_MSG + "SM4_1";
            public static final String SM_OFF = BT_MSG + "SM4_0";
            public static final String SM_SPEED = BT_MSG + "SM4";
        }
    }

    public static char getCharFromString(String str, int index) {
        return str.charAt(index);
    }
}
