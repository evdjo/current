package cs22510_2014;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class GPSReader {

	protected class Location {
		public double latitude;
		public double longitude;
		public Date date;

		public String toString() {
			return date.toString() + "\n" + latitude + "," + longitude;
		}
	}

	protected final static String EOF = "EOF";
	protected final static String GPS_TIME = "GPS_TIME";
	protected final static String SATELITE = "SATELITE";
	protected final static String GSV = "GSV";
	protected final static String RMC = "RMC";

	protected final static double MIL = 1000000.0;

	protected Location currLoc;
	protected Date currTime;
	protected boolean satelitesOK;

	private BufferedReader br;

	public GPSReader(String fileName) {
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
			e.printStackTrace();
		}

		this.currLoc = degreesToDecimal(thedata[3], thedata[5]);

		if (thedata[4].equals("S"))
			this.currLoc.latitude *= -1;

		if (thedata[6].equals("W"))
			this.currLoc.longitude *= -1;

		this.currLoc.date = currTime;

	}

	private Location degreesToDecimal(String latitude, String longitude) {

		Location loc = new Location();

		double lat = Double.parseDouble(latitude);
		int lat_degrees = (int) lat / 100;
		double lat_minutes = (lat - lat_degrees * 100) / 60.0;

		double lng = Double.parseDouble(longitude);
		int lng_degrees = (int) lng / 100;
		double lng_minutes = (lng - lng_degrees * 100) / 60.0;

		loc.latitude = Math.round((lat_minutes + lat_degrees) * MIL) / MIL;
		loc.longitude = Math.round((lng_minutes + lng_degrees) * MIL) / MIL;

		return loc;
	}
}
