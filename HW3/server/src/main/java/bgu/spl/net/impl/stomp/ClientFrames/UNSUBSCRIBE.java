package bgu.spl.net.impl.stomp.ClientFrames;

import bgu.spl.net.impl.stomp.Frame;

public class UNSUBSCRIBE extends Frame
{
    private int id;
    private int receipt;

    public UNSUBSCRIBE(int id, int receipt)
    {
        this.id = id;
        this.receipt = receipt;
    }

    public int getId() {return id;}
    public int getReceipt() {return receipt;}

    @Override
    public String toString() {return null;}
}
