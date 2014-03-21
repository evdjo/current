package sorting2014;
/**
 *  Quick Sort combined with Insertion Sort.
 * @author Evdzhan Mustafa enm3@aber.ac.uk
 *
 */
@SuppressWarnings("rawtypes")
public class QuickSort_Hybrid implements Sorter {

	private static final int INSERT_SORT = 10; // when to start using insertion sort
    private Comparable[] items;
	@Override
	public void sort(Comparable[] items, int cutoff) {
		this.items = items;
		quicksort(0, items.length - 1);
	}

	
	/**
	 * Sorts the element in array items. An item from the array is selected
	 * based on some criteria as a "pivot". The method works by finding the
	 * correct position of the pivot in the array.
	 * 
	 * The items that are less than the pivot are moved to the the left sub
	 * array. The items that are greater than the pivot are stored on the right
	 * of the pivot.
	 * 
	 * The last two steps do not assure that the the left or right sub arrays
	 * are sorted, but rather assure that the pivot element is in it's correct
	 * and final place. This means that the pivot position will change (although
	 * it might not).
	 * 
	 * After the pivot's correct position is found, a recursive call is
	 * performed to the right, and the left sub-arrays.
	 * 
	 * @param items     The (sub)array to be sorted
	 * @param first	     the index of the first item in the (sub)array
	 * @param last      the index of the last item in the (sub)array
	 */
	private void quicksort(int first, int last) {

		if (last - first <= INSERT_SORT) { // use insert sort if array size below
											// the limit
			this.insertionsort(first, last);
			return;
		}

		int pivot = this.findMedian(first, last); // pivot middle element

		pivot = setPivot(first, last, pivot); // moves the pivot to it's
														// correct position

		if (first < pivot)
			quicksort( first, pivot - 1); // recursively sort the sub
												// array
												// left from the pivot
		if (last > pivot)
			quicksort(pivot + 1, last); // recursively sort the sub array
												// right from the pivot

	}

	/**
	 * A helper-method. Works by placing the pivot in it's correct position.
	 * 
	 * Elements with index lower than the pivot, are less than the pivot's
	 * value. Elements with index higher than the pivot , are greater than the
	 * pivot's value.
	 * 
	 * The last two statements DO NOT state that the left and right sub arrays
	 * are themselves sorted.
	 * 
	 * @return returns the updated index of the pivot
	 */
	private int setPivot(int left, int right, int pivotIndex) {

		swap(pivotIndex, left); // move the pivot out of the way

		int swap = left + 1; // set the initial swap position

		for (int i = swap; i <= right; ++i) { // loop through swap until
												// right(inclusive)

			if (items[left].compareTo(items[i]) > 0) { // compare the current
														// item with the pivot

				swap( swap, i ); // swap the current item with the swap
										// position
				
				swap++; // move the swap position to the right
			}

		}
		swap(left, swap - 1); // put the pivot to it's right place
		return swap - 1; // swap - 1 is the pivot's new index

	}

	/**
	 * A helper-method. Swaps two elements from the array items. The integers
	 * indexFirst and indexSecond are the indexes of the elements to be swapped.
	 * 
	 * @param items         the array where the swap will occur
	 * @param indexFirst    the index of the first element
	 * @param indexSecond   the index of the second element
	 */
	private void swap(int indexFirst, int indexSecond) {

		Comparable temp = items[indexFirst]; // store the first in temporary
												// variable

		items[indexFirst] = items[indexSecond]; // change the first to the
												// second's value

		items[indexSecond] = temp; // change the second to the first's value

	}

	/**
	 * A helper-method. Finds the median among the first, middle and last
	 * element of the current array or sub array.
	 * 
	 * @param items      the primary array where the data lives
	 * @param first      index of the first element
	 * @param last       index of the last element
	 * @return the index of the median of the three
	 */
	@SuppressWarnings("unchecked")
	private int findMedian(int first, int last) {

		int pivot;
		int mid = first + (last - first) / 2;

		if (items[first].compareTo(items[mid]) > 0) {

			if (items[first].compareTo(items[last]) < 0)
				pivot = first;
			
			else if (items[mid].compareTo(items[last]) > 0)
				pivot = mid;
			
			else pivot = last;
			
		} else {
			
			if (items[mid].compareTo(items[last]) < 0)
				pivot = mid;
			
			else if (items[first].compareTo(items[last]) > 0)
				pivot = first;
			
			else pivot = last;

		}
		return pivot;
	}
	/**
	 * @see InsertionSort 
	 */
	private void insertionsort(int start, int end) {

		for (int i = start; i <= end; ++i) {

			Comparable temp = items[i]; // the current element to compare

			int j; // integer to index the shifted value

			for (j = i - 1; j >= 0; --j) {

				if (items[j].compareTo(temp) < 0) {
					break; // the current position is correct - stop shifting
				}
				items[j + 1] = items[j]; // continually shift the previous value
											// to current position
			}
			items[j + 1] = temp; // insert the value to it's correct position
		}
	}
	

}
