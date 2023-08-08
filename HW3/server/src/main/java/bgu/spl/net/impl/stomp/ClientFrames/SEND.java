package bgu.spl.net.impl.stomp.ClientFrames;

import bgu.spl.net.impl.stomp.Frame;

public class SEND extends Frame
{
    private String destination;
    private String message;
    private int receipt;

    public SEND(String destination,String message, int receipt)
    {
        this.destination = destination;
        this.message = message;
        this.receipt = receipt;
    }
    
    public String getDestination() {return destination;}
    public String getMessageFrame() {return message;}
    public int getReceipt() {return receipt;}

    @Override
    public String toString() {return null;}
}
