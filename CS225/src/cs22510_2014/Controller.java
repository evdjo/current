package cs22510_2014;

import java.util.LinkedList;

public class Controller {
	Model strm1, strm2;
	LinkedList<Location> locs;
	Location offset;

	public Controller(String file1, String file2) {

		strm1 = new Model(file1);
		strm2 = new Model(file2);
		locs = new LinkedList<>();
	}

	public boolean synchronizeTimes() {
		String data = "";
		while (!data.equals(Model.GPS_TIME) && !data.equals(Model.EOF)) {
			data = strm1.read();

		}
		if (data.equals(Model.EOF)) {
			return false;
		}

		data = "";
		while (!data.equals(Model.GPS_TIME) && !data.equals(Model.EOF)) {
			data = strm2.read();
		}
		if (data.equals(Model.EOF)) {
			return false;
		}

		int time = strm1.currTime.compareTo(strm2.currTime);

		if (time == -1) {
			do {
				data = strm1.read();
				if (data == Model.EOF)
					return false;
				if (data.equals(Model.GPS_TIME)) {
					time = strm1.currTime.compareTo(strm2.currTime);

				}
			} while (time != 0);

		} else if (time == 1) {
			do {
				data = strm2.read();
				if (data == Model.EOF)
					return false;
				if (data.equals(Model.GPS_TIME)) {
					time = strm1.currTime.compareTo(strm2.currTime);

				}
			} while (time != 0);

		}
		this.offset = strm1.currLoc.getOffset(strm2.currLoc);
		return true;
	}

	public void start() {
		this.synchronizeGPS();
		this.synchronizeTimes();
		while (true) {

			if (strm1.satelitesOK) {
				if (strm1.currTime.toString().equals(
						("Fri Feb 21 15:14:20 GMT 2014"))) {
					System.out.println("asd");
				}
				locs.add(strm1.currLoc);
				if (strm2.satelitesOK)
					this.offset = strm1.currLoc.getOffset(strm2.currLoc);

				else if (this.offset != null)
					strm2.currLoc = Location.getLoc(this.offset, strm1.currLoc);

			} else if (strm2.satelitesOK) {
				locs.add(strm2.currLoc);
				if (this.offset != null)
					strm1.currLoc = Location.getLoc(this.offset, strm2.currLoc);

			}
			String lineRead = "";
			do {
				lineRead = strm1.read();
				if (lineRead.equals(Model.EOF))
					return;

			} while (!lineRead.equals(Model.GPS_TIME));
			lineRead = "";
			do {
				lineRead = strm2.read();
				if (lineRead.equals(Model.EOF))
					return;

			} while (!lineRead.equals(Model.GPS_TIME));

		}

	}

	public boolean synchronizeGPS() {
		String data = "";
		if (!strm1.satelitesOK) {
			while (!strm1.satelitesOK) {
				data = strm1.read();
				if (data == Model.EOF)
					return false;
			}
		}
		if (!strm2.satelitesOK) {
			while (!strm2.satelitesOK) {
				data = strm2.read();

				if (data == Model.EOF)
					return false;
			}
		}

		return true;
	}

}
