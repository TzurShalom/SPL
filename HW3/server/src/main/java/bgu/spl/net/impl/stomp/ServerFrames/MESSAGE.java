package bgu.spl.net.impl.stomp.ServerFrames;

import bgu.spl.net.impl.stomp.Frame;

public class MESSAGE extends Frame
{
    private int subscription;
    private int messageId;
    private String destination;
    private String message;

    public MESSAGE(int subscription, int messageId, String destination, String message) 
    {
        this.subscription = subscription;
        this.messageId = messageId;
        this.destination = destination;
        this.message = message;
    }

    public void setSubscription(int subscriptionId) {subscription = subscriptionId;}

    public String toString()
    {
        return "MESSAGE\n" + 
        "subscription:" + subscription + "\n" +
        "message-id:" + messageId + "\n" +
        "destination:" + destination + "\n" +
        "\n" +
        message;
    }
}
