package cs22510_2014;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class Model {

	final static String EOF = "EOF";
	final static String GPS_TIME = "GPS_TIME";
	final static String SATELITE = "SATELITE";
	final static String GSV = "GSV";
	final static String RMC = "RMC";

	Location currLoc;
	Date currTime;
	boolean satelitesOK;

	private BufferedReader br;

	public Model(String fileName) {

		try {
			br = new BufferedReader(new FileReader(fileName));
		} catch (IOException e) {}
		
		satelitesOK = false;

	}

	protected String read() {
		String data = "";
		try {
			while (true) {
				data = br.readLine();
				if (data == null)
					return EOF;

				if (data.equals(""))
					continue;

				String type = data.substring(3, 6);

				if (type.equals(GSV)) {
					int linesNum = Integer.parseInt(data.substring(7, 8));

					String[] sateliteData = new String[linesNum];

					sateliteData[0] = data;

					for (int i = 1; i < linesNum; i++) {
						sateliteData[i] = br.readLine();
					}
					proccessGSV(sateliteData);

					return SATELITE;

				} else if (type.equals(RMC)) {

					proccessRMC(data);

					return GPS_TIME;
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return EOF;

	}

	private void proccessGSV(String[] gsvData) {

		int satNum = Integer.parseInt(gsvData[0].substring(11, 13));
		if (satNum < 3) {
			this.satelitesOK = false;
			return;
		}

		int count = 0;

		for (int i = 0; i < gsvData.length; i++) {
			int end = gsvData[i].indexOf('*');
			String[] data = gsvData[i].substring(14, end).split(",");
			for (int o = 3; o < 16 && satNum > 1 && o < data.length; o += 4) {

				if (!data[o].equals("") && Integer.parseInt(data[o]) > 30) {
					count++;
					if (count == 3) {
						
						this.satelitesOK = true;
						return;
					}
				}
				satNum--;
			}

		}
		this.satelitesOK = false;
		 

	}

	private void proccessRMC(String rmcData) {

		String[] thedata = rmcData.split(",");
		String thetime = thedata[1].substring(0, 6);
		String thedate = thedata[9];

		try {
			currTime = new SimpleDateFormat("HHmmssddMMyy", Locale.ENGLISH)
					.parse(thetime + thedate);

		} catch (ParseException e) {
			System.out.println("lol");
		}

		this.currLoc = degreesToDecimal(thedata[3], thedata[5]);
		if(thedata[4].equals("S")) this.currLoc.latitude *=-1; 
		if(thedata[6].equals("W")) this.currLoc.longitude *=-1; 
		 
		this.currLoc.date = currTime;

	}

	static Location degreesToDecimal(String latitude, String longitude) {

		Location loc = new Location();
		BigDecimal bd = new BigDecimal(Double.parseDouble(latitude));
        bd  = bd.setScale(5,RoundingMode.CEILING);
		BigDecimal degrees = bd.divide(new BigDecimal(100));
		
		
	/*	
		
		long data1 = (long) (Double.parseDouble(latitude)*10000);
		
		int degrees1 = (int) data1 / 1000000;		
		int minutes1 = (int) (data1 - degrees1*1000000) / 60;
		
		double minutes1_ = (minutes1 / 10000.0);
		
		loc.latitude = degrees1 + minutes1_;

		
		
		long data2 = (long)(Double.parseDouble(longitude)*10000);
		
		int degrees2 = (int) data2 / 1000000;
		int minutes2 = (int) (data2 - degrees2*1000000) / 60 ;
	
		double minutes2_ = (minutes2 / 10000.0);

		loc.longitude = degrees2 + minutes2_;
*/
		return loc;
	}
}
