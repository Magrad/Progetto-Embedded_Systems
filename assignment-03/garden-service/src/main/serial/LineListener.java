package main.serial;

import main.Pair;

public interface LineListener {

    /**
     * Gets the current values of the component settings.
     */
    public Pair<String, Pair<Integer,Integer>>[] getComponentList();
    
    
    /**
     * Gets the setting of a single component.
     * @param id of the component needed
     */
    public Pair<String, Pair<Integer,Integer>> getComponent(int id);
    
    
    /**
     * Gets the current system state.
     */
    public String getAuthority();
    
    
    /**
     * Gets if the Arduino setup has been completed.
     */
    public boolean isSetupCompleted();
    
    
    /**
     * Sends to the Arduino the current temperature and
     * luminosity retrieved from the ESP32 sensor board.
     * @param temp current temperature
     * @param day current luminosity
     * @throws Exception
     */
    public void sendSerialTempAndLuminosity(int temp, int day) throws Exception;
    
    
    /**
     * Function used to send a closing message on the Serial Line.
     */
    public void sendSerialConnectionClosed();
}
