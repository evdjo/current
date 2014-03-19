 
package sorting2014;


/**
 * Improved bubble sort. Based on the bubble sort by Richard Shipman.
 * @author Evdzhan Mustafa enm3@aber.ac.uk
 *
 */
public class ImprovedBubbleSort implements Sorter {

	 
	@SuppressWarnings("unchecked")
	public void sort(Comparable[] items, int cutoff) {
		Comparable temp;
		
		boolean flag=true;   // declare the flag 
		
		for (int i = 0; i < items.length - 1; i++) {
			
			
			 flag=true;  // assume the array is sorted
			
			for (int j = 0; j < items.length - 1 - i; j++) {
				if (items[j].compareTo(items[j + 1]) > 0) {
					temp = items[j + 1];
					items[j + 1] = items[j];
					items[j] = temp;
					flag=false;  //swap happened , array not sorted
				}
			}
			
			if(flag)  {  break;  }  // no swaps occurred exit 
			
		}
	}
}

