package bgu.spl.net.srv;
import java.util.List;
import bgu.spl.net.impl.stomp.User;

public interface Connections<T> 
{
    boolean send(int connectionId, T msg); 

    void send(List<User> users, String channel, T msg) ; 

    void disconnect(int connectionId);

    void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler);

    void removeConnectionHandler(int connectionId);

    int getMessageId();
}
