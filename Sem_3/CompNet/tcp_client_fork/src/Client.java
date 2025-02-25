import java.io.*;
import java.net.Socket;
import java.util.Scanner;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Client {
    public static void main(String[] args) {
        String ip="10.152.3.92";
        int port=1234;
        try(Socket clientSocket=new Socket(ip,port);

            OutputStream outputStream=clientSocket.getOutputStream();
            DataOutputStream dataOutputStream = new DataOutputStream(outputStream);
            Scanner scanner=new Scanner(System.in);

            PrintWriter writer=new PrintWriter(outputStream,true);
            InputStream inputStream = clientSocket.getInputStream();
            DataInputStream dataInputStream=new DataInputStream(inputStream);
            BufferedReader reader=new BufferedReader(new InputStreamReader(inputStream)))
        {
//            System.out.print("Enter first number:");
//            int number1=scanner.nextInt();
//            System.out.print("Enter second number:");
//            int number2=scanner.nextInt();
//
//            //send nr
//            dataOutputStream.writeInt(number1);
//           // dataOutputStream.writeInt(number2);
//            dataOutputStream.flush();;
//            dataOutputStream.writeInt(number2);
//            dataOutputStream.flush();
            int n=scanner.nextInt();
            dataOutputStream.writeInt(n);
            dataOutputStream.flush();
            for (int i=0;i<n;i++)
            {
                int x=scanner.nextInt();
                dataOutputStream.writeInt(x);
                dataOutputStream.flush();
            }
            //recv nr
            int sum=dataInputStream.readInt();
            System.out.println(sum);

//            String response=reader.readLine();
//            if (response != null) {
//                //int len=Integer.parseInt(response);
//                System.out.println("Received from server: " + response);
//            }


        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }
}