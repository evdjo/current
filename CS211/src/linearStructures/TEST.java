package linearStructures;

public class TEST {
	static public int findMedian(Comparable[] items, 
			int first,
			int sec,			
			int third) {
		
		 if (items[first].compareTo(items[sec]) > 0) {
			
			 if (items[first].compareTo(items[third]) < 0)return first;			
			 else if (items[sec].compareTo(items[third]) > 0)return sec;
			 else return third;
		}
		 else { 
			if(items[sec].compareTo(items[third]) < 0) return sec; 
			else if(items[first].compareTo(items[third]) > 0)return first;
			else return third;
 
		} 
	}
	
	static private Comparable[] a = {
			0, 1 , 2 , 3 , 4 ,5 , 6
	};
	public static void main(String[] args) {
		System.out.println(a[findMedian(a,2,3,1)]);
	}
}
