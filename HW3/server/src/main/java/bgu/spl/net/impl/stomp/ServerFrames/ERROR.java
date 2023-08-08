package bgu.spl.net.impl.stomp.ServerFrames;

import bgu.spl.net.impl.stomp.Frame;

public class ERROR extends Frame
{
    private String header;

    public ERROR(String header) 
    {
        this.header = header;
    }

    public String toString() 
    {
        return "ERROR\n" +
        "message:" + header + "\n" +
        "\n";
    }
}
