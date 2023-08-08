package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Users
{
    public ConcurrentHashMap<String, User> usernameToUser;
    public ConcurrentHashMap<Integer, User> connectionIdToUser;      
    public ConcurrentHashMap<String, ConcurrentLinkedQueue<User>> channelToSubscribers;

    public Users()
    {
        usernameToUser = new ConcurrentHashMap<String, User>();
        connectionIdToUser = new ConcurrentHashMap<Integer, User>();
        channelToSubscribers = new ConcurrentHashMap<String, ConcurrentLinkedQueue<User>>();
    }
}
