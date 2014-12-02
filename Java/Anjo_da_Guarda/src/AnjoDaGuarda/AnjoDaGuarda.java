package AnjoDaGuarda;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Enumeration;

public class AnjoDaGuarda implements SerialPortEventListener {

    private SerialPort serialPort;
    private static final String PORT_NAMES[] = {"/dev/tty.usbmodem1411", // Mac
        // OS
        // X
        "/dev/ttyACM0", // Raspberry Pi
        "/dev/ttyUSB0", // Linux
        "COM3", // Windows
};
    private BufferedReader input;
    private OutputStream output;
    private static final int TIME_OUT = 2000;
    private static final int DATA_RATE = 9600;
    private static final int CLOSE_FIRE = 0;
    private static final int DISTANT_FIRE = 1;
    private static final int MOVEMENT = 2;

    public static void main(String[] args) throws Exception {
        AnjoDaGuarda anjo = new AnjoDaGuarda();
        anjo.initialize();
        Thread t = new Thread() {
            public void run() {
                try {
                    Thread.sleep(86400000);//24h
                } catch (InterruptedException ie) {
                }
            }
        };
        t.start();
        System.out.println("Started");
    }

    public void initialize() {
        System.setProperty("gnu.io.rxtx.SerialPorts", "/dev/tty.usbmodem1411");
        CommPortIdentifier portId = null;
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();
        // First, Find an instance of serial port as set in PORT_NAMES.
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum
                    .nextElement();
            for (String portName : PORT_NAMES) {
                if (currPortId.getName().equals(portName)) {
                    portId = currPortId;
                    break;
                }
            }
        }
        if (portId == null) {
            System.out.println("Could not find COM port.");
            return;
        }
        try {
            // open serial port, and use class name for the appName.
            serialPort = (SerialPort) portId.open(this.getClass().getName(),
                    TIME_OUT);
            // set port parameters
            serialPort.setSerialPortParams(DATA_RATE, SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            // open the streams
            input = new BufferedReader(new InputStreamReader(
                    serialPort.getInputStream()));
            output = serialPort.getOutputStream();
            // add event listeners
            serialPort.addEventListener(this);
            serialPort.notifyOnDataAvailable(true);
        } catch (Exception e) {
            System.err.println(e.toString());
        }
    }

    public synchronized void close() {
        if (serialPort != null) {
            serialPort.removeEventListener();
            serialPort.close();
        }
    }

    /**
     * Handle an event on the serial port. Read the data and print it.
     */
    public synchronized void serialEvent(SerialPortEvent oEvent) {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                String inputLine = input.readLine();
                actionByString(inputLine);
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }
		// Ignore all the other eventTypes, but you should consider the other
        // ones.
    }

    private static void actionByString(String inputLine) {
        Integer i = new Integer(inputLine);
        switch (i) {
            case CLOSE_FIRE:
                System.out.println("Fogo proximo!");
                break;
            case DISTANT_FIRE:
                System.out.println("Fogo distante ou luz solar!");
                break;
            case MOVEMENT:
                System.out.println("Movimento detectado");
                break;
        }
    }

}
