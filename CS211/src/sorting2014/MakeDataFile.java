/*
 * Created on Nov 29, 2003
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package sorting2014;
import java.io.*;

/**
 * @author rcs
 *
 */
public class MakeDataFile {


	public static void main(String[] args) {
		if (args.length == 2) {
			makeFile(args[0],Integer.parseInt(args[1]));
		} else {
			System.out.println("Creating lots of files, please wait... dots will get faster");
			for (int i = 7 ; i < 8; i++) {
				System.out.print(".");
				makeFile("test"+String.valueOf(i)+".dat", (int)Math.pow(10,i));
				if(i == 8) break;
				System.out.print(".");
				makeFile("test"+String.valueOf(i)+"a"+".dat", (int)Math.pow(10,i)*2);
				System.out.print(".");
				makeFile("test"+String.valueOf(i)+"b"+".dat", (int)Math.pow(10,i)*4);
				System.out.print(".");
				makeFile("test"+String.valueOf(i)+"c"+".dat", (int)Math.pow(10,i)*6);
				System.out.print(".");
				makeFile("test"+String.valueOf(i)+"d"+".dat", (int)Math.pow(10,i)*8);
			}
			System.out.println("done");
		}
	}
		
	public static void makeFile(String name, int numbers)
	{
		BufferedWriter writer=null;
		try {
			writer = new BufferedWriter(new FileWriter(name));
		} catch (IOException ex) {
		}

		int number=0;
		
		for (int i = 0; i<numbers; i++)
		{
			
			number=(int)(1000000+(Math.random()*8999999));
			try {
				writer.write(number+"\n");
			} catch (IOException e)
			{}
		}

		try {
			writer.close();
		} catch (IOException ex) {
		}
	}
	
	
	
	
}
