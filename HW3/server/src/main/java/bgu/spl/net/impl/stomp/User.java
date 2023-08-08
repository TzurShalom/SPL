package bgu.spl.net.impl.stomp;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class User 
{
    private String username;
    private String password;
    private int connectionId;
    private boolean active = false;
    private Map<String,Integer> channelToId;
    private int id;

    public User(String username, String password, int connectionId)
    {
        this.username = username;
        this.password = password;
        this.connectionId = connectionId;
        this.channelToId = new ConcurrentHashMap<String,Integer>();
        this.id = 1;
    }

    public String getUsername() {return username;}
    public String getPassword() {return password;}
    public int getConnectionId() {return connectionId;}
    public void setConnectionId(int connectionHandlerId) {connectionId = connectionHandlerId;}
    public boolean getActive() {return active;}
    public void setActive(boolean state) {active = state;}
    public void addChannel(String channel) {channelToId.putIfAbsent(channel, id); id++;}
    public void removeChannel(String channel) {channelToId.remove(channel);}

    public int getIdByChannel(String channel) 
    {
        if (channelToId.get(channel) != null) {return channelToId.get(channel);}
        else {return -1;}
    }
    
    public String getChannelById(int id)
    {
        for (String c : channelToId.keySet())
        {
            if (channelToId.get(c) == id) {return c;}
        }
        return null;
    }
}
