package serialArduino.src;

import javax.swing.*;
import java.util.*;
import java.awt.*;
import java.awt.event.ActionListener;

public class GUI extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private static final Pair<Integer,Integer> FILL = new Pair<>(0,0);
    private static final Pair<Integer,Integer> RESTART = new Pair<>(2,0);
    private static final Pair<Integer,Integer> TESTS = new Pair<>(0,3);
    private static final Pair<Integer,Integer> STATUS = new Pair<>(2,3);
    private final String FILL_MSG = "Fill ";
    private final String RESTART_MSG = "Restart ";
    private final String TEST_MSG = "Test ";
    private final String STATE_MSG = "State ";
    private final String COFFEE_MSG = "Coffee ";
    private final String TEA_MSG = "Tea ";
    private final String CHOCOLATE_MSG = "Chocolate ";

    private final Map<JButton,Pair<Integer,Integer>> btn_grid = new HashMap<>();
    private final Map<JLabel,Pair<Integer,Integer>> lbl_grid = new HashMap<>();

    public enum State {
        IDLE("Idle"),
        WORKING("Working"),
        ASSITANCE("Assistance"),
        NONE("None");

        private final String label;

        private State(String label) {
            this.label = label;
        }
    }

    private enum Drink {
        COFEE("Coffee"),
        TEA("Tea"),
        CHOCOLATE("Chocolate");

        private final String label;

        private Drink(String label) {
            this.label = label;
        }
    }

    private int nTests = 0;
    private State currState = State.NONE;
    private int N_MAX = 0;
    private int[] drinks = new int[] { N_MAX, N_MAX, N_MAX };
 
    public GUI(int size, CommChannel channel) {
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setSize(100*size, 100*size);

        JPanel panel = new JPanel(new GridLayout(size,size));
        this.getContentPane().add(panel);

        ActionListener al = e -> {
            var button = (JButton)e.getSource();
            var position = btn_grid.get(button);

            if(position.equals(FILL)) {
                channel.sendMsg(FILL_MSG);
            } 
            if(position.equals(RESTART)) {
                channel.sendMsg(RESTART_MSG);
            }

            this.updateVisual();
        };
      
        for (int y=0;y<size;y++){
            for (int x=0;x<3;x++){
                final JButton jb = new JButton(" ");
                final JLabel jl = new JLabel(" ", SwingConstants.CENTER);
                jb.addActionListener(al);
                if(new Pair<>(x,y).equals(FILL) || new Pair<>(x,y).equals(RESTART)) {
                    panel.add(jb);
                    this.btn_grid.put(jb, new Pair<>(x,y));
                } else {
                    panel.add(jl);
                    this.lbl_grid.put(jl, new Pair<>(x,y));
                }
            }
        }
        this.setVisible(true);
        this.updateVisual();
        this.clock(channel);
    }
    
    private void updateVisual() {
        this.btn_grid.forEach((b,p) -> {
            if(p.equals(FILL)) {
                b.setText("Refill");
            }
            if(p.equals(RESTART)) {       
                b.setText("Recover");
            }            
        });
        this.lbl_grid.forEach((l,p) -> {
            if(p.equals(new Pair<>(0,1))) {       
                l.setText("" + Drink.COFEE.label);
            }
            if(p.equals(new Pair<>(1,1))) {       
                l.setText("" + Drink.TEA.label);
            }
            if(p.equals(new Pair<>(2,1))) {       
                l.setText("" + Drink.CHOCOLATE.label);
            }
            if(p.equals(new Pair<>(0,2))) {       
                l.setText("" + drinks[0]);
            }
            if(p.equals(new Pair<>(1,2))) {       
                l.setText("" + drinks[1]);
            }
            if(p.equals(new Pair<>(2,2))) {       
                l.setText("" + drinks[2]);
            }
            if(p.equals(TESTS)) {
                l.setText("Check Tests:");
            }
            if(p.equals(new Pair<>(0,4))) {
                l.setText("" + nTests);
            }
            if(p.equals(STATUS)) {
                
                l.setText("Check Status:");
            }
            if(p.equals(new Pair<>(2,4))) {
                l.setText("" + currState.label);
            }  
        });
    }
    
    private void clock(CommChannel channel) {
        Thread clock = new Thread() {            
            public void run() {
                try {
                    for(;;) {
                        if(channel.isMsgAvailable()) {
                            String msg = channel.receiveMsg();
                            if(msg.contains(TEST_MSG)) {
                                msg = msg.replaceAll("[^0-9.]", "");
                                nTests = Integer.parseInt(msg);
                            } else if(msg.contains(STATE_MSG)) {
                                msg = msg.replace(STATE_MSG, "");
                                currState = msg.equals(GUI.State.IDLE.label) ? GUI.State.IDLE : msg.equals(GUI.State.WORKING.label) ? GUI.State.WORKING : msg.equals(GUI.State.ASSITANCE.label) ? GUI.State.ASSITANCE : currState;
                            } else if(msg.contains(FILL_MSG)) {
                                msg = msg.replaceAll("[^0-9.]", "");
                                N_MAX = Integer.parseInt(msg);
                                for(int i=0;i<drinks.length;i++) {
                                    drinks[i] = N_MAX;
                                }
                            } else if(msg.contains(COFFEE_MSG)) {
                                msg = msg.replaceAll("[^0-9.]", "");
                                drinks[0] = Integer.parseInt(msg);
                            } else if(msg.contains(TEA_MSG)) {
                                msg = msg.replaceAll("[^0-9.]", "");
                                drinks[1] = Integer.parseInt(msg);
                            } else if(msg.contains(CHOCOLATE_MSG)) {
                                msg = msg.replaceAll("[^0-9.]", "");
                                drinks[2] = Integer.parseInt(msg);
                            }
                            
                            updateVisual();
                        }
                    }
                } catch(InterruptedException e) {
                }
            }
            
        };
        
        clock.start();
    }
    
}
