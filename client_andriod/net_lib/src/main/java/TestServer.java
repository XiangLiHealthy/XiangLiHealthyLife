import java.io.*;
import java.net.*;

public class TestServer {

    public static void main(String[] args) {
        int port = 6666;
        try {
            ServerSocket server = new ServerSocket(port);
            Socket socket = server.accept();
            Reader reader = new InputStreamReader(socket.getInputStream());
            char chars[] = new char[1024];
            int len;
            StringBuilder builder = new StringBuilder();
            while ((len=reader.read(chars)) != -1) {
                builder.append(new String(chars, 0, len));
            }
            System.out.println("Receive from client message=: " + builder);
            reader.close();
            socket.close();
            server.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
