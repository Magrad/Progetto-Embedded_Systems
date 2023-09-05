package main;

public class Config {
    
    public static int CharToInt(char ch) {       
        return ch - '0';
    }
    
    public static final String JSON_FILE = "log.json";
    public static final String JSON_SAVE_FILE = "save.json";
    public static final String LED_N_NAME = "led_";
    public static final String LED_1_NAME = "led_1";
    public static final String LED_2_NAME = "led_2";
    public static final String LED_3_NAME = "led_3";
    public static final String LED_4_NAME = "led_4";
    public static final String SERVO_NAME = "irrigation";
    public static final String NONE = "NONE";
    public static final String AUTO = "AUTO";
    public static final String MANUAL = "MANUAL";
    public static final String NOMINAL = "NOMINAL";
    public static final String ALARM = "ALARM";
    public static final int ON_OFF_LEDS = 2;
    public static final int BRIGHTNESS_LEDS = 2;
    public static final int HW_COMPONENTS = 5;
    
    
    public static class Codes {
        public static final String OK = "OK";
        public static final String ERR = "ERR";
        public static final String BT_CONNECTION_CLOSED = "CC";
    }
    
    public static class SerialReciever {
        public static final String SL_MSG = "SL:";
        public static final String AUTO = SL_MSG + "AUTO";
        public static final String MANUAL = SL_MSG + "MANUAL";
        public static final String ALARM = SL_MSG + "ALARM";
        public static final String NOMINAL = SL_MSG + "NOMINAL";
        public static final String SETUP = SL_MSG + "SETUP";
        public static final String LED_N = SL_MSG + "LED_";
        public static final String IRRIGATION = SL_MSG + "SM";
        public static final String COMPONENT = SL_MSG + "CMP:";
        public static final String TEMPERATURE = SL_MSG + "TMP:";
        public static final String LUMINOSITY = SL_MSG + "LUM:";
    }
    
    public static class MQTTAgent {
        public static final String MQTT_MSG = "MQTT:";
        public static final String ALARM = MQTT_MSG + "ALARM";
        public static final String NOMINAL = MQTT_MSG + "NOMINAL";
        public static final String TEMPERATURE = "Temperature";
        public static final String LUMINOSITY = "Luminosity";
    }
}
