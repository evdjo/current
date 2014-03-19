package cs22510_2014;

import java.util.Date;

public class Location {
	public double latitude;
	public double longitude;
    public Date date;
	public String toString() {
		return latitude + "," + longitude;
	}

	 
	public Location getOffset(Location l) {
		Location loc = new Location();

		long lata = (long) (l.latitude * 100000);
		long latb = (long) (this.latitude * 100000);

		loc.latitude = (lata - latb);

		long lnga = (long) (l.longitude * 100000);
		long lngb = (long) (this.longitude * 100000);

		loc.longitude = (lnga - lngb);

		return loc;
	}

	static public Location getLoc(Location offset,Location other) {
		Location loc = new Location();
		loc.latitude = other.latitude;
	    loc.longitude = other.longitude;
	    
	    loc.latitude  = (((long)(loc.latitude * 100000)) +offset.latitude)  / 100000.0;
		
	    loc.longitude  = (((long) (loc.longitude *100000)) +offset.longitude)/ 100000.0;
	    return  loc;
	   
	}
}
