package main;

import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JFrame;

import io.vertx.core.Vertx;
import main.mqtt.MQTTAgent;
import main.serial.CommChannel;
import main.serial.SerialCommChannel;
import main.serial.SerialLineListener;

public class SmartGardenManager {
    
    public static void main(String[] args) throws Exception {
        CommChannel channel = new SerialCommChannel(args[0],9600);

        System.out.println("Waiting Arduino for rebooting...");         
        Thread.sleep(3000);
        System.out.println("Ready.");
        
        JSONLogic jslogic = new JSONLogic();
        
        // Receive the current state of the system.
        Pair<String, Pair<String,Pair<Integer,Integer>>[]> json = jslogic.readOrCreate("./" + Config.JSON_FILE, "./" + Config.JSON_SAVE_FILE);
        
        String authority = json.getX();
        Pair<String,Pair<Integer,Integer>>[] componentsList = json.getY();
        
        // Initialize the serial and mqtt listeners.
        SerialLineListener listener = new SerialLineListener(authority, componentsList, channel, jslogic);
        MQTTAgent agent = new MQTTAgent(jslogic, listener);

        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(agent);
        
        /* Adds the two listeners to the jslogic in order to retrieve
         * data to then write to a JSON file, used as storage for old
         * and new settings and to populate the web app.
         */
        jslogic.setListeners(listener, agent);
        
        // Creates a GUI to quickly access the current system informations
        GUI gui = new GUI(5, listener);
        /* Adds a listener used run some function before the program
         * closure.
         */        
        gui.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentHidden(ComponentEvent e) {         
                try {
                    jslogic.restoreFreshJSON();
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
                
                /* Lets the Arduino know that new settings cannot be
                 * accepted due to the fact that the service app is
                 * currently offline.
                 */
                listener.sendSerialConnectionClosed();
                ((JFrame)(e.getComponent())).dispose();
                
                // Closes the program.
                System.exit(0);
            }
        });
    }
}
