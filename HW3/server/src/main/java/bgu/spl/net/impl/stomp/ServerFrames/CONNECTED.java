package bgu.spl.net.impl.stomp.ServerFrames;

import bgu.spl.net.impl.stomp.Frame;

public class CONNECTED extends Frame
{
    private String version;

    public CONNECTED(String version)
    {
        this.version = version;
    }

    public String toString()
    {
        return "CONNECTED\n" + 
        "version:" + version + "\n" +
        "\n";
    }
}