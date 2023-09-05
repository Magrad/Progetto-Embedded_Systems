package main.mqtt;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.mqtt.MqttClient;
import main.Config;
import main.JSONLogic;
import main.serial.SerialLineListener;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;


/**
 * Class used to send and retrieve informations from the
 * ESP32 sensor board that sends information over MQTT
 * protocol.
 * @author Andrea
 *
 */
public class MQTTAgent extends AbstractVerticle {
    private final Integer DEAD_CHARACTERS = 2;
    
    private int day = -1;
    private int temperature = -1;
    private String alarm;
    private boolean alarmChanged;
    
    private JSONLogic jslogic;
    private SerialLineListener listener;

    /**
     * Constructor
     * @param logic, JSON logic used to send informations needed from the system
     * @param listener, the Serial Listener that communicates with Arduino
     */
    public MQTTAgent(JSONLogic logic, SerialLineListener listener) {
        this.jslogic = logic;
        this.listener = listener;
        this.alarm = this.jslogic.getState();
        this.alarmChanged = true;
    }

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        /*
         * The client subscribes using the topic relative to
         * this project in order to listen or send data on
         * this channel.
         */
        client.connect(1883, "broker.mqtt-dashboard.com", c -> {

                log("connected");
                
                log("subscribing...");
                
                client.publishHandler(s -> {
                  System.out.println("MQTT topic:" + s.topicName() + " - " + s.payload().toString());
                  
                  int change = 0;
                  
                  // If the message contains the string "Temperature" reads its value.
                  if(s.payload().toString().contains(Config.MQTTAgent.TEMPERATURE)) {
                      String msg = s.payload().toString();
                      
                      msg = msg.replace(msg.substring(0, msg.indexOf(Config.MQTTAgent.TEMPERATURE) + Config.MQTTAgent.TEMPERATURE.length() + DEAD_CHARACTERS), "");
                      this.temperature = Integer.parseInt(msg);
                      
                      change += 1;
                  }
                  
                  // If the message contains the string "Luminosity" reads its value.
                  if(s.payload().toString().contains(Config.MQTTAgent.LUMINOSITY)) {
                      String msg = s.payload().toString();
                      
                      msg = msg.replace(msg.substring(0, msg.indexOf(Config.MQTTAgent.LUMINOSITY) + Config.MQTTAgent.LUMINOSITY.length() + DEAD_CHARACTERS), "");
                      this.day = Integer.parseInt(msg);
                      
                      change += 2;
                  }
                  
                  // Notifies the sensor board with the current alarm state.
                  if(this.alarmChanged) {
                      log("publishing a msg");
                      // The client publishes a the current alarm state.
                      client.publish("smart-garden-2122",
                              Buffer.buffer(this.alarm),
                              MqttQoS.AT_LEAST_ONCE,
                              false,
                              false);
                      this.alarmChanged = false;
                  }
                  
                  try {
                      // Only when both values are read the content is send to the serial listener.
                      if(change == 2) {
                          listener.sendSerialTempAndLuminosity(this.temperature, this.day);
                          change = 0;
                      }
                  } catch (Exception e) {
                      // Continue
                  }
                  
                })
                .subscribe("smart-garden-2122", 2);
        });
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
    
    /**
     * Gets the current day state read from the sensor.
     */
    public Integer getDayState() {
        return this.day;
    }
    
    /**
     * Gets the current temperature read from the sensor.
     */
    public Integer getTemperature() {
        return this.temperature;
    }
    
    /**
     * Sets the current alarm variable to the value passed
     * as argument.
     */
    public void setAlarm(String value) {
        this.alarm = value;
        this.alarmChanged = true;
    }
}