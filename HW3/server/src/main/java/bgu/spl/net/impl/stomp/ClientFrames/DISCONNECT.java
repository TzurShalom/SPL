package bgu.spl.net.impl.stomp.ClientFrames;

import bgu.spl.net.impl.stomp.Frame;

public class DISCONNECT extends Frame
{
    private int receipt;

    public DISCONNECT(int receipt)
    {
        this.receipt = receipt;
    }

    public int getReceipt() {return receipt;}

    @Override
    public String toString() {return null;}
}
