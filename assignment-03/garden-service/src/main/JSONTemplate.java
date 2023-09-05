package main;

import main.mqtt.MQTTAgent;
import main.serial.SerialLineListener;

/**
 * Interface.
 * @author Andrea
 *
 */
public interface JSONTemplate {
    
    /**
     * Initializes the two listener. 
     * @param listener the Serial Listener that communicates with Arduino
     * @param agent the MQTT Listener that communicates with the sensor board
     */
    public void setListeners(SerialLineListener listener, MQTTAgent agent);
    
    
    /**
     * Function that returns the system settings.
     * @param filename is the JSON file to read or write from
     * @param savefile is the JSON file used to save the operator phone settings
     * @return the current system state: AUTO, MANUAL or ALARM to indicate who
     * has authority to make changes to the system. During automatic control the
     * system operates on its own. On manual control the operator dictates the
     * current system settings and can clear an alarm status from the system.
     * During an alarm state only the operator can access the system in order
     * to restore its nominal state.
     * Returns as an Y value of the components settings.
     * @throws Exception
     */
    public Pair<String, Pair<String, Pair<Integer,Integer>>[]> readOrCreate(String filename, String savefile) throws Exception;
    
    
    /**
     * Function that clears the content of the JSON file created at the
     * launch of the program and restores the settings values to their
     * respective default states.
     * @throws Exception
     */
    public void restoreFreshJSON() throws Exception;
    
    
    /**
     * Function called to save current settings from serial listener.
     * @param filename the JSON file name
     * @throws Exception
     */
    public void buildLogJSONFile(String filename) throws Exception;
    
    
    /**
     * Function used to retrieve saved settings from a phone app
     * during manual request.
     * @return the components settings
     * @throws Exception
     */
    public Pair<String, Pair<Integer,Integer>>[] getSaveJSONFile() throws Exception;
    
    
    /**
     * Sets the value passed as argument as the current system state
     * (AUTO, MANUAL, ALARM).
     * @param value the given value to change to.
     */
    public void setState(String value);
    
    
    /**
     * Gets the current system state value.
     */
    public String getState();
}
