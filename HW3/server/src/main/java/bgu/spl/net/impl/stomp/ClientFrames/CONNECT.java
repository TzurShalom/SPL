package bgu.spl.net.impl.stomp.ClientFrames;

import bgu.spl.net.impl.stomp.Frame;

public class CONNECT extends Frame
{
    private String acceptVersion;
    private String host;
    private String username;
    private String password;
    private int receipt;

    public CONNECT(String acceptVersion, String host, String username, String password, int receipt)
    {
        this.username = username;
        this.password = password;
        this.host = host;
        this.acceptVersion = acceptVersion;
        this.receipt = receipt;
    }

    public String getAcceptVersion() {return acceptVersion;}
    public String getHost() {return host;}
    public String getUsername() {return username;}
    public String getPassword() {return password;}
    public int getReceipt() {return receipt;}

    @Override
    public String toString() {return null;}
}
