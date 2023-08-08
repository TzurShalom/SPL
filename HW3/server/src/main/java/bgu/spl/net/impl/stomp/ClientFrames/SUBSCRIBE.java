package bgu.spl.net.impl.stomp.ClientFrames;

import bgu.spl.net.impl.stomp.Frame;

public class SUBSCRIBE extends Frame
{
    private String destination;
    private int id;
    private int receipt;

    public SUBSCRIBE(String destination, int id, int receipt)
    {
        this.destination = destination;
        this.id = id;
        this.receipt = receipt;
    }

    public String getDestination() {return destination;}
    public int getId() {return id;}
    public int getReceipt() {return receipt;}

    @Override
    public String toString() {return null;}
}
