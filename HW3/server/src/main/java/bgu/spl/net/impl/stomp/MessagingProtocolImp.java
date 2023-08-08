package bgu.spl.net.impl.stomp;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.stomp.ClientFrames.*;
import bgu.spl.net.impl.stomp.ServerFrames.*;
import bgu.spl.net.srv.Connections;

public class MessagingProtocolImp implements MessagingProtocol<Frame>
{
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<Frame> connections;
    private Users users;

    @Override
    public void start(int connectionId, Connections<Frame> connections, Users users)
    {
        this.connectionId = connectionId;
        this.connections = connections;
        this.users = users;
    }

    @Override
    public Frame process(Frame message) 
    {
        if (message instanceof CONNECT) //---------------------------------------------------------------------------//
        {
            CONNECT connectFrame = (CONNECT) message;
            String username = connectFrame.getUsername();

            if (connectFrame.getAcceptVersion().equals("1.2"))
            {
                if (users.usernameToUser.containsKey(username))
                {
                    User user = users.usernameToUser.get(username);
    
                    if (user.getPassword().equals(connectFrame.getPassword())) 
                    {
                        if (!user.getActive()) 
                        {
                            user.setActive(true); 

                            users.connectionIdToUser.remove(user.getConnectionId(), user);
                            users.connectionIdToUser.put(connectionId, user);
                            user.setConnectionId(connectionId);
                            
                            CONNECTED connectedFrame = new CONNECTED(connectFrame.getAcceptVersion());
                            connections.send(connectionId, connectedFrame);
                        }
                        else
                        {
                            user.setActive(false); 
                            ERROR errorFrame = new ERROR("User is already logged in");
                            connections.send(connectionId, errorFrame); 
                            //connections.disconnect(connectionId);
                        }
                    }
                    else
                    {
                        user.setActive(false); 
                        ERROR errorFrame = new ERROR("Wrong password");
                        connections.send(connectionId, errorFrame); 
                        //connections.disconnect(connectionId);
                    }
                }
                else // Create a new user and connect him to the server
                {
                    User user = new User(username, connectFrame.getPassword(), connectionId);
                    user.setActive(true); 
                    users.usernameToUser.putIfAbsent(username, user);
                    users.connectionIdToUser.putIfAbsent(connectionId, user);
    
                    CONNECTED connectedFrame = new CONNECTED(connectFrame.getAcceptVersion());
                    connections.send(connectionId, connectedFrame); 
                }
            }
            else
            {
                ERROR errorFrame = new ERROR("The versions are not compatible");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
        } 
        else if (message instanceof DISCONNECT) //---------------------------------------------------------------------------//
        {
            User user = users.connectionIdToUser.get(connectionId);

            if (!user.getActive())
            {
                ERROR errorFrame = new ERROR("The user is not logged in");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
            else
            {
                user.setActive(false); 
                DISCONNECT disconnectFrame = (DISCONNECT) message;
            
                synchronized(this)  // Removing all the client's subscriptions
                {
                    for (String channel : users.channelToSubscribers.keySet())
                    {
                        if (users.channelToSubscribers.get(channel).contains(user))
                        {
                            users.channelToSubscribers.get(channel).remove(user);
                        }
                    }
                }
                
                RECEIPT receiptFrame = new RECEIPT(disconnectFrame.getReceipt());
                connections.send(connectionId, receiptFrame); 
                //connections.disconnect(connectionId);
            }
        }
        else if (message instanceof SEND) //---------------------------------------------------------------------------//
        {
            User user = users.connectionIdToUser.get(connectionId);
            SEND sendFrame = (SEND) message;

            if (!user.getActive())
            {
                ERROR errorFrame = new ERROR("The user is not logged in");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
            else
            {
                if (users.channelToSubscribers.containsKey(sendFrame.getDestination()) && // Does this channel exist?
                users.channelToSubscribers.get(sendFrame.getDestination()).contains(user)) // Is the user subscribed to this channel?
                {
                    MESSAGE messageFrame = new MESSAGE(user.getIdByChannel(sendFrame.getDestination()), connections.getMessageId(), sendFrame.getDestination(), sendFrame.getMessageFrame());

                    List<User> list = new ArrayList<>();
                    String channel = sendFrame.getDestination();
                    for (User u : users.channelToSubscribers.get(channel)) {list.add(u);}
                    connections.send(list, channel, messageFrame); 

                    if (sendFrame.getReceipt() != -1) 
                    {
                        RECEIPT receiptFrame = new RECEIPT(sendFrame.getReceipt());
                        connections.send(connectionId, receiptFrame); 
                    }
                }
                else
                {
                    user.setActive(false); 
                    ERROR errorFrame = new ERROR("The user is not subscribed to this channel");
                    connections.send(connectionId, errorFrame); 
                }
            }
        }      
        else if (message instanceof SUBSCRIBE) //---------------------------------------------------------------------------//
        {
            User user = users.connectionIdToUser.get(connectionId);
            SUBSCRIBE subscribeFrame = (SUBSCRIBE) message;

            if (!user.getActive())
            {
                user.setActive(false); 
                ERROR errorFrame = new ERROR("The user is not logged in");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
            else
            {
                users.channelToSubscribers.putIfAbsent(subscribeFrame.getDestination(), new ConcurrentLinkedQueue<User>()); // In case the channel does not exist
                users.channelToSubscribers.get(subscribeFrame.getDestination()).add(users.connectionIdToUser.get(connectionId));
                users.connectionIdToUser.get(connectionId).addChannel(subscribeFrame.getDestination());
    
                RECEIPT receiptFrame = new RECEIPT(subscribeFrame.getReceipt());
                connections.send(connectionId, receiptFrame); 
            }
        }  
        else if (message instanceof UNSUBSCRIBE) //---------------------------------------------------------------------------//
        {
            User user = users.connectionIdToUser.get(connectionId);
            UNSUBSCRIBE unsubscribeFrame = (UNSUBSCRIBE) message;

            if (!user.getActive())
            {
                user.setActive(false); 
                ERROR errorFrame = new ERROR("The user is not logged in");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
            else if (unsubscribeFrame.getId() == -1)
            {
                user.setActive(false); 
                ERROR errorFrame = new ERROR("The user is not subscribed to this channel");
                connections.send(connectionId, errorFrame); 
                //connections.disconnect(connectionId);
            }
            else
            {
                users.channelToSubscribers.get(user.getChannelById(unsubscribeFrame.getId())).remove(user);
                users.connectionIdToUser.get(connectionId).removeChannel(user.getChannelById(unsubscribeFrame.getId()));
    
                RECEIPT receiptFrame = new RECEIPT(unsubscribeFrame.getReceipt());
                connections.send(connectionId, receiptFrame);
            }
        }   
        else if (message instanceof ERROR) //---------------------------------------------------------------------------//
        {
            connections.send(connectionId, (ERROR) message); 
            //connections.disconnect(connectionId);
        }   
        return null;
    }

    @Override
    public boolean shouldTerminate() 
    {
        return shouldTerminate;
    }
}
