package main;

import javax.swing.*;

import main.serial.SerialLineListener;

import java.util.*;
import java.awt.*;

public class GUI extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private static final Pair<Integer,Integer> LED_1 = new Pair<>(0,0);
    private static final Pair<Integer,Integer> STATE = new Pair<>(1,0);
    private static final Pair<Integer,Integer> LED_2 = new Pair<>(2,0);
    private static final Pair<Integer,Integer> LED_3 = new Pair<>(0,1);
    private static final Pair<Integer,Integer> LED_3_BRIGHTNESS = new Pair<>(0,2);
    private static final Pair<Integer,Integer> LED_4 = new Pair<>(2,1);
    private static final Pair<Integer,Integer> LED_4_BRIGHTNESS = new Pair<>(2,2);
    private static final Pair<Integer,Integer> IRRIG = new Pair<>(1,3);
    private static final Pair<Integer,Integer> IRRIG_SPEED = new Pair<>(1,4);

    private final Map<JLabel,Pair<Integer,Integer>> lbl_grid = new HashMap<>();
    private final SerialLineListener listener;
 
    public GUI(int size, SerialLineListener listener) {
        this.listener = listener;
        
        // Initializes the GUI display.
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setSize(100*size, 100*size);

        JPanel panel = new JPanel(new GridLayout(size,size));
        this.getContentPane().add(panel);
      
        for (int y=0;y<size;y++){
            for (int x=0;x<3;x++){
                final JLabel jl = new JLabel(" ", SwingConstants.CENTER);
                panel.add(jl);
                this.lbl_grid.put(jl, new Pair<>(x,y)); 
            }
        }
        
        this.setVisible(true); 
        this.selfUpdate();
        
        // Sets the on close event to, in reality, hide the GUI.
        this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
    }
    
    /**
     * Function used to update the current displayed items by getting
     * new informations from the serial listener.
     */
    private void updateVisual() {
        this.lbl_grid.forEach((l,p) -> {
            Pair<String, Pair<Integer,Integer>> component;
            if(p.equals(LED_1)) {
                component = listener.getComponent(0);
                l.setText("" + component.getX());
                l.setForeground(component.getY().getX() == 0 ? Color.red : Color.green);
            }
            if(p.equals(STATE)) {
                String authority = listener.getAuthority();
                l.setText("" + authority);
                l.setForeground(authority == "ALARM" ? Color.red : Color.black);
            }            
            if(p.equals(LED_2)) {
                component = listener.getComponent(1);
                l.setText("" + component.getX());
                l.setForeground(component.getY().getX() == 0 ? Color.red : Color.green);
            }
            if(p.equals(LED_3)) {
                component = listener.getComponent(2);
                l.setText("" + component.getX());
                l.setForeground(component.getY().getX() == 0 ? Color.red : Color.green);
            }
            if(p.equals(LED_3_BRIGHTNESS)) {
                component = listener.getComponent(2);
                l.setText("" + component.getY().getY());
            }
            if(p.equals(LED_4)) {
                component = listener.getComponent(3);
                l.setText("" + component.getX());
                l.setForeground(component.getY().getX() == 0 ? Color.red : Color.green);
            }
            if(p.equals(LED_4_BRIGHTNESS)) {
                component = listener.getComponent(3);
                l.setText("" + component.getY().getY());
            }
            if(p.equals(IRRIG)) {
                component = listener.getComponent(4);
                l.setText("" + component.getX());
                l.setForeground(component.getY().getX() == 0 ? Color.red : Color.green);
            }
            if(p.equals(IRRIG_SPEED)) {
                component = listener.getComponent(4);
                l.setText("" + component.getY().getY());
            }
        });
    }
    
    /**
     * Function used to self update the GUI every 0.5 seconds. 
     */
    private void selfUpdate() {
        Thread clock = new Thread() {            
            public void run() {
                try {
                    for(;;) {
                        if(listener.isSetupCompleted()) {
                            updateVisual();
                            Thread.sleep(500);
                        }
                    }
                } catch(InterruptedException e) {
                }
            }
            
        };
        
        clock.start();
    }
}
