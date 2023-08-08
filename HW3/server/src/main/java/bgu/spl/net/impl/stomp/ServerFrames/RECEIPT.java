package bgu.spl.net.impl.stomp.ServerFrames;

import bgu.spl.net.impl.stomp.Frame;

public class RECEIPT extends Frame
{
    private int receiptId;

    public RECEIPT(int receiptId)
    {
        this.receiptId = receiptId;
    }

    @Override
    public String toString()
    {
        return "RECEIPT\n" + 
        "receipt-id:" + receiptId + "\n" +
        "\n";
    }
}
