package serialArduino.src;

public class CoffeeMachineManager {
    public static void main(String[] args) throws Exception {
        CommChannel channel = new SerialCommChannel(args[0],9600);

        System.out.println("Waiting Arduino for rebooting...");         
        Thread.sleep(4000);
        System.out.println("Ready.");

        new GUI(5, channel);
    }
}
