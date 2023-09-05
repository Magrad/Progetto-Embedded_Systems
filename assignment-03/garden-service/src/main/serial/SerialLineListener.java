package main.serial;

import main.Config;
import main.JSONLogic;
import main.Pair;

/**
 * Class used to send and retrieve informations from
 * Arduino as a Serial Line message.
 * @author Andrea
 *
 */
public class SerialLineListener implements LineListener {
    
    private String authority;
    private final boolean setup;
    private boolean serialLineSetup;
    
    private int last_temp = -1;
    private int last_light = -1;
    
    private Pair<String, Pair<Integer,Integer>>[] components;
    private final CommChannel channel;
    private final JSONLogic jslogic;
    
    /**
     * Constructor.
     * @param authority the current system state
     * @param components_list list of all the components in the system
     * @param channel the Serial Line channel in which the message will be sent or retrieved
     * @param jslogic, JSON logic used to send informations needed from the system
     */
    public SerialLineListener(String authority, Pair<String, Pair<Integer,Integer>>[] components_list, CommChannel channel, JSONLogic jslogic) {
        this.authority = authority;
        this.setup = true;
        this.serialLineSetup = false;
        this.components = components_list;
        this.channel = channel;
        this.jslogic = jslogic;
        
        this.startSerialLineListener();
    }
    
    @Override
    public Pair<String, Pair<Integer,Integer>>[] getComponentList() {
        return components;
    }
    
    @Override
    public Pair<String, Pair<Integer,Integer>> getComponent(int id) {
        return this.components[id];
    }
    
    @Override
    public String getAuthority() {
        return this.authority;
    }
    
    @Override
    public boolean isSetupCompleted() {
        return this.setup;
    }
    
    @Override
    public void sendSerialTempAndLuminosity(int temp, int day) throws Exception {
        if(this.serialLineSetup) {
            if(last_temp != temp) {
                last_temp = temp;
                channel.sendMsg(Config.SerialReciever.TEMPERATURE + last_temp);
            }
            Thread.sleep(125);
            if(last_light != day) {
                last_light = day;
                channel.sendMsg(Config.SerialReciever.LUMINOSITY + last_light);
            }
        }
    }
    
    @Override
    public void sendSerialConnectionClosed() {
        if(this.serialLineSetup) {
            channel.sendMsg(Config.SerialReciever.SL_MSG + Config.Codes.BT_CONNECTION_CLOSED);
        }
    }
    
    private void startSerialLineListener() {
        Thread clock = new Thread() {            
            public void run() {
                try {
                    for(;;) {
                        // Checks if there is a message in queue.
                        if(channel.isMsgAvailable()) {
                            String msg = channel.receiveMsg();
                            
                            System.out.println("RECEIVED:" + msg);
                            boolean changed = false;
                            
                            // Checks if Arduino requires setup.
                            if(msg.contains(Config.SerialReciever.SETUP)) {
                                serialLineSetup = false;
                                                                
                                for(int i=0;i<Config.HW_COMPONENTS;i++) {
                                    channel.sendMsg(Config.SerialReciever.SL_MSG + i + "_" + components[i].getY().getX() + "_" + components[i].getY().getY());
                                    
                                    Thread.sleep(500);
                                }
                                
                                channel.sendMsg("SL:DONE:" + authority);
                                serialLineSetup = true;
                            }
                            
                            // Checks if one operator is requiring control.
                            if(msg.contains(Config.SerialReciever.MANUAL)) {
                                msg = msg.replace(Config.SerialReciever.SL_MSG, "");
                                authority = authority != Config.ALARM ? msg : authority;
                                
                                Pair<String, Pair<Integer,Integer>>[] savedSettings = jslogic.getSaveJSONFile();
                                
                                for(int i=0;i<Config.HW_COMPONENTS;i++) {
                                    channel.sendMsg(Config.SerialReciever.SL_MSG + "SAVE:" +
                                                    i + "_" + savedSettings[i].getY().getX() + "_" + savedSettings[i].getY().getY());
                                    
                                    Thread.sleep(500);
                                }
                                
                                channel.sendMsg((String)(Config.SerialReciever.SL_MSG + Config.Codes.OK));
                                Thread.sleep(500);
                                
                                
                                changed = true;
                            }
                            
                            /* Checks if the system has to return to automatic state,
                             * usually after the operator has closed his app.
                             */
                            if(msg.contains(Config.SerialReciever.AUTO)) {
                                msg = msg.replace(Config.SerialReciever.SL_MSG, "");
                                authority = authority != Config.ALARM ? msg : authority;
                                channel.sendMsg(Config.SerialReciever.SL_MSG + Config.Codes.BT_CONNECTION_CLOSED);
                                jslogic.buildLogJSONFile(Config.JSON_SAVE_FILE);
                            }
                            
                            // Checks if the Arduino is sending an alarm message.
                            if(msg.contains(Config.SerialReciever.ALARM)) {
                                jslogic.setState(Config.ALARM);
                                authority = Config.ALARM;
                                changed = true;
                            }
                            
                            // Checks if the Arduino is restoring the alarm status.
                            if(msg.contains(Config.SerialReciever.NOMINAL)) {
                                jslogic.setState(Config.NOMINAL);
                                authority = Config.MANUAL;
                                changed = true;
                            }
                            
                            /* Checks if the Arduino is requiring the system to change
                             * the settings of a Led.
                             */
                            if(msg.contains(Config.SerialReciever.LED_N)) {
                                msg = msg.replace(Config.SerialReciever.LED_N, "");
                                msg = msg.replace("_", "");
                                int id = Config.CharToInt(msg.charAt(0));
                                String name = Config.LED_N_NAME + (id + 1);
                                int state;
                                int brightness;
                                
                                if(id < Config.ON_OFF_LEDS) {
                                    state = Config.CharToInt(msg.charAt(1));          
                                    brightness = state == 0 ? 0 : 4;
                                } else {
                                    brightness = Config.CharToInt(msg.charAt(1));
                                    state = brightness == 0 ? 0 : 1;
                                }
                                
                                components[id] = new Pair<>(name, new Pair<>(state, brightness));         
                                
                                String post = Config.SerialReciever.COMPONENT + Config.Codes.OK;
                                
                                changed = true;
                                channel.sendMsg(post);
                            }
                            
                            /* Checks if the Arduino is requiring the system to change
                             * the settings of the Servo motor.
                             */
                            if(msg.contains(Config.SerialReciever.IRRIGATION)) {
                                msg = msg.replace(Config.SerialReciever.IRRIGATION, "");
                                msg = msg.replace("_", "");
                                int id = Config.CharToInt(msg.charAt(0));
                                String name = Config.SERVO_NAME;
                                int state = Config.CharToInt(msg.charAt(1));
                                int brightness = Config.CharToInt(msg.charAt(2));
                                
                                components[id] = new Pair<>(name, new Pair<>(state, brightness));
                                
                                String post = Config.SerialReciever.COMPONENT + Config.Codes.OK;
                                
                                changed = true;
                                channel.sendMsg(post);
                            }
                            
                            /*
                             * Checks if values that requires an update of the JSON file
                             * have been changed. For example a component or the current
                             * status of the system.
                             */
                            if(changed) {
                                jslogic.buildLogJSONFile("./" + Config.JSON_FILE);
                            }
                        }
                    }
                } catch(Exception e) {
                }
            }
            
        };
        
        clock.start();
    }
}
