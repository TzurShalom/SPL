package bgu.spl.net.srv;

import java.io.IOException;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import bgu.spl.net.impl.stomp.User;
import bgu.spl.net.impl.stomp.ServerFrames.MESSAGE;

public class ConnectionsImp<T> implements Connections<T>
{
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectionIdToConnectionHandler =
        new ConcurrentHashMap<Integer, ConnectionHandler<T>>();

    private static AtomicInteger messageId = new AtomicInteger(0);
    
    @Override
    public boolean send(int connectionId, T msg)
    {
        ConnectionHandler<T> connectionHandler = connectionIdToConnectionHandler.get(connectionId);

        if (connectionHandler != null) 
        {
            connectionHandler.send(msg);
            return true;
        }
        return false;
    }

    @Override
    public void send(List<User> users, String channel, T msg) 
    {
        for (User user : users)
        {
            if (msg instanceof MESSAGE) {((MESSAGE) msg).setSubscription(user.getIdByChannel(channel));}
            connectionIdToConnectionHandler.get(user.getConnectionId()).send(msg);
        }         
    }

    @Override
    public void disconnect(int connectionId)
    {
        try {connectionIdToConnectionHandler.get(connectionId).close();}
        catch (IOException e) {};
        removeConnectionHandler(connectionId);
    }
    
    @Override
    public void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler)
    {
        connectionIdToConnectionHandler.putIfAbsent(connectionId, connectionHandler);
    }

    @Override
    public void removeConnectionHandler(int connectionId)
    {
        connectionIdToConnectionHandler.remove(connectionId);
    }

    @Override
    public int getMessageId()
    {
        return messageId.incrementAndGet();
    }
}
