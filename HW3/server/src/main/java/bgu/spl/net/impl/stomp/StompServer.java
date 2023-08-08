package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer 
{
    public static void main(String[] args) 
    {
        int port = Integer.parseInt(args[0]);
        String model = args[1];

        if (model.equals("tpc"))
        {
            Server.threadPerClient(
                port, 
                () -> new MessagingProtocolImp(), 
                MessageEncoderDecoderImp::new 
            ).serve();
        }
        else if (model.equals("reactor"))
        {
            Server.reactor(
                Runtime.getRuntime().availableProcessors(),
                port, 
                () -> new MessagingProtocolImp(), 
                MessageEncoderDecoderImp::new 
        ).serve();
        }
    }
}
