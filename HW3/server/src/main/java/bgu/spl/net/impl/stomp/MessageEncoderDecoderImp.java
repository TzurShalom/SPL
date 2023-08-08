package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.ClientFrames.*;
import bgu.spl.net.impl.stomp.ServerFrames.ERROR;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.HashMap;

public class MessageEncoderDecoderImp implements MessageEncoderDecoder<Frame>
{
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;

    public String convertByteToString(byte nextByte)
    {
        if (nextByte == '\u0000') {return popString();}
        pushByte(nextByte);
        return null; 
    }

    public String[] convertStringToArray(String text) 
    {
        text = text.replaceAll("\n\n", "\nmessage:\n");
        String[] lines = text.split("\n");
        return lines;
    }

    public HashMap<String,String> convertArrayToHashMap(String[] lines)
    {
        HashMap<String,String> hashMap = new HashMap<>();

        int i = 0;
        if(lines[i] != null) {hashMap.put("stomp-command",lines[i]);}
        else{return null;}
        i++;

        String[] header;
        String key;
        String value;

        while(i<lines.length) // headers
        {
            if (lines[i].equals("message:")) {break;}
            header = lines[i].split(":");
            key = header[0];
            if (header.length > 1) {value = header[1];}
            else {value = "";}
            hashMap.put(key, value);
            i++;
        }

        if (i < lines.length) // message
        {
            i++;
            String message = "";

            while(i < lines.length)
            {
                message = message + "\n" + lines[i];
                i++;
            }

            hashMap.put("message", message);
        }

        return hashMap;
    }

    @Override
    public Frame decodeNextByte(byte nextByte) // byte -> string -> frame
    {
        String text = convertByteToString(nextByte);

        if (text != null && text != "null")
        {
            String[] lines = convertStringToArray(text);
       
            HashMap<String,String> hashMap = convertArrayToHashMap(lines);
            int receipt = -1;
            
            System.out.println(hashMap.get("stomp-command"));
            switch (hashMap.get("stomp-command")) 
            {
                case "CONNECT":
                {
                    String[] connectHeaders = {"accept-version", "host", "login", "passcode"};

                    for (String header : connectHeaders)
                    {
                        if (!hashMap.containsKey(header))
                        {
                            return new ERROR("The " + header + " header is missing from the CONNECT frame");
                        }
                    }

                    if (hashMap.containsKey("receipt")) {receipt = Integer.parseInt(hashMap.get("receipt"));} //

                    return new CONNECT(hashMap.get("accept-version"),
                        hashMap.get("host"),
                        hashMap.get("login"),
                        hashMap.get("passcode"),
                        receipt); 
                }

                case "DISCONNECT":
                {
                    if (!hashMap.containsKey("receipt")) {return new ERROR("The receipt header is missing from the DISCONNECT frame");}         
                    receipt = Integer.parseInt(hashMap.get("receipt"));
                    return new DISCONNECT(receipt); 
                }

                case "SEND":
                {
                    if (!hashMap.containsKey("destination")) {return new ERROR("The destination header is missing from the SEND frame");}
                    else if (!hashMap.containsKey("message")) {return new ERROR("The message body is missing from the SEND frame");}

                    if (hashMap.containsKey("receipt")) {receipt = Integer.parseInt(hashMap.get("receipt"));}
                    return new SEND(hashMap.get("destination"),hashMap.get("message"),receipt);
                }

                case "SUBSCRIBE":
                {
                    String[] subscribeHeaders = {"destination", "id"};

                    for (String header : subscribeHeaders)
                    {
                        if (!hashMap.containsKey(header))
                        {
                            return new ERROR("The " + header + " header is missing from the SUBSCRIBE frame");
                        }
                    }

                    if (!hashMap.containsKey("receipt")) {return new ERROR("The receipt header is missing from the SUBSCRIBE frame");}

                    receipt = Integer.parseInt(hashMap.get("receipt"));

                    return new SUBSCRIBE(hashMap.get("destination"),
                    Integer.parseInt(hashMap.get("id")),
                    receipt); 
                }

                case "UNSUBSCRIBE":
                {
                    if (!hashMap.containsKey("id")) {return new ERROR("The id header is missing from the UNSUBSCRIBE frame");}
                    else if (!hashMap.containsKey("receipt")) {return new ERROR("The receipt header is missing from the UNSUBSCRIBE frame");}
                    receipt = Integer.parseInt(hashMap.get("receipt"));
                    return new UNSUBSCRIBE(Integer.parseInt(hashMap.get("id")),receipt); 
                }
            }
        }
        return null;
    }

    @Override
    public byte[] encode(Frame message)
    {
        return (message.toString() + "\u0000").getBytes();
    }

    private void pushByte(byte nextByte)
    {
        if (len >= bytes.length) {bytes = Arrays.copyOf(bytes, len * 2);}
        bytes[len++] = nextByte;
    }

    private String popString()
    {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        return result;
    }

}
