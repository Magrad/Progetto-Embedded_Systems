package main;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import main.mqtt.MQTTAgent;
import main.serial.SerialLineListener;

/**
 * Class used to save and load JSON file from the file system.
 * Functions from this class are called each time the service
 * app comes online and every time it goes offline. Every time
 * an operator requires manual control, in order to display the
 * last saved variables on the phone app.
 * @author Andrea
 *
 */
public class JSONLogic implements JSONTemplate {
    
    private String filename;
    private String saveFile;
    
    private String authority;
    private String state;
    private Pair<String, Pair<Integer,Integer>>[] componentsList;
    
    private SerialLineListener listener;
    private MQTTAgent agent;
    
    /**
     * The constructor initializes every settings to
     * their respective default values.
     */
    public JSONLogic() {
        this.authority = Config.AUTO;
        this.state = Config.NOMINAL;
        
        /* Each component is made of their name and a pair of
         * values that indicate that component current state.
         * The first value (X value), holds the current state
         * of the component, On or Off. The second one (Y value),
         * holds the current intensity, value from a certain range
         * depending on the component. 
         */
        this.componentsList = new Pair[] {
                new Pair<>(Config.LED_1_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_2_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_3_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_4_NAME, new Pair<>(0,0)),
                new Pair<>(Config.SERVO_NAME, new Pair<>(0,0)),
        };        
    }

    @Override
    public void setListeners(SerialLineListener listener, MQTTAgent agent) {
        this.listener = listener;
        this.agent = agent;
    }
    
    @Override
    public Pair<String, Pair<String, Pair<Integer,Integer>>[]> readOrCreate(String filename, String savefile) throws Exception {
        this.filename = filename;
        this.saveFile = savefile;
        
        File f = new File(this.filename);
        JSONArray componentsJSON = null;
        
        if(!f.exists() && !f.isDirectory()) {
            FileWriter file = new FileWriter(f);
            componentsJSON = buildLogJSONFileFromScratch();
            file.write(componentsJSON.toJSONString());
            file.flush();
        } else {
            FileReader file = new FileReader(f);
            JSONParser jsonParser = new JSONParser();
            Object obj = jsonParser.parse(file);
            componentsJSON = (JSONArray) obj;
            componentsJSON.forEach(entry -> parseLogsJSONObject((JSONObject)entry));
            
            restoreFreshJSON();
        }
        
        return new Pair<>(authority, this.componentsList);
    }
    
    /**
     * Function that retrieves system settings from a JSON Object
     * passed as argument.
     * @param logs the JSON Object that contains every setting
     */
    private void parseLogsJSONObject(JSONObject logs) {        
        JSONObject component = (JSONObject) logs.get("component");
        
        if(component != null) {
            int id = ((Long)component.get("id")).intValue();
            String name = (String) component.get("name");
            int onOff = ((Long)component.get("state")).intValue();
            int intensity = ((Long)component.get("intensity")).intValue();
            
            this.componentsList[id] = new Pair<>(name, new Pair<>(onOff, intensity));
        } else {
            this.state = (String) logs.get("state");
            this.authority = this.state.equals(Config.ALARM) ? Config.ALARM : this.authority;
        }
    }

    /**
     * Function that creates a new JSON file from scratch and returns
     * it content in order to print it on file.
     * @return a JSON Object that will be used a data to write on the
     * desired file.
     */
    private JSONArray buildLogJSONFileFromScratch() {        
        JSONArray jsonToFile = new JSONArray();
        JSONObject stateJSON = new JSONObject();
        
        stateJSON.put("state", this.state);
        
        jsonToFile.add(stateJSON);
        for(int i=0;i<this.componentsList.length;i++) {
            JSONObject capsule = new JSONObject();
            JSONObject componentJSON = new JSONObject();
            componentJSON.put("id", i);
            componentJSON.put("name", this.componentsList[i].getX());
            componentJSON.put("state", this.componentsList[i].getY().getX());
            componentJSON.put("intensity", this.componentsList[i].getY().getY());
            
            capsule.put("component", componentJSON);
            jsonToFile.add(capsule);
        }

        return jsonToFile;
    }
    
    @Override
    public void restoreFreshJSON() throws Exception {
        this.componentsList = new Pair[] {
                new Pair<>(Config.LED_1_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_2_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_3_NAME, new Pair<>(0,0)),
                new Pair<>(Config.LED_4_NAME, new Pair<>(0,0)),
                new Pair<>(Config.SERVO_NAME, new Pair<>(0,0)),
        };
        
        File f = new File(this.filename);
        FileWriter file = new FileWriter(f, false);
        
        JSONArray jsonToFile = buildLogJSONFileFromScratch(); 
        
        file.write(jsonToFile.toJSONString());
        file.flush();
    }
    
    @Override
    public void buildLogJSONFile(String filename) throws Exception {
        JSONArray jsonToFile = new JSONArray();
        
        this.componentsList = this.listener.getComponentList();
        
        File f = new File(filename);
        FileWriter file = new FileWriter(f, false);
        
        jsonToFile = buildLogJSONFileFromScratch();
        
        file.write(jsonToFile.toJSONString());
        file.flush();
    }
    
    @Override
    public Pair<String, Pair<Integer,Integer>>[] getSaveJSONFile() throws Exception {
        File f = new File(this.saveFile);        
        FileReader file = new FileReader(f);
        
        JSONArray componentsJSON = null;
        JSONParser jsonParser = new JSONParser();
        Object obj = jsonParser.parse(file);
        componentsJSON = (JSONArray) obj;
        
        String tmpState = this.state;
        String tmpAuthority = this.authority;
        
        componentsJSON.forEach(entry -> parseLogsJSONObject((JSONObject)entry));     
        
        this.state = tmpState;
        this.authority = tmpAuthority;
        
        return this.componentsList;
    }
    
    @Override
    public void setState(String value) {
        this.state = value;
        this.agent.setAlarm(this.state);
    }
    
    @Override
    public String getState() {
        return this.state;
    }
}
