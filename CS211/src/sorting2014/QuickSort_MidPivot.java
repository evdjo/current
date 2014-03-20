package sorting2014;

@SuppressWarnings("rawtypes")
/**
 * Quick sort where the pivot is chosed to be the Mid element.
 * @author Evdzhan Mustafa enm3@aber.ac.uk
 *
 */
public class QuickSort_MidPivot implements Sorter {
private Comparable[] items ;
	@Override
	public void sort(Comparable[] items, int cutoff) {
		
		this.items = items ;
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
	 * @param items       The (sub)array to be sorted
	 * @param first       the index of the first item in the (sub)array
	 * @param last        the index of the last item in the (sub)array
	 */
	private void quicksort( int first, int last) {

		if (last <= first) {
			return; // if only one item in the array - it is sorted
		}

		int pivot = first + (last - first) / 2; // pivot middle element

		pivot = setPivot(first, last, pivot); // moves the pivot to it's
														// correct position

		if (first < pivot)
			quicksort(first, pivot - 1); // recursively sort the sub
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

				swap(swap, i); // swap the current item with the swap
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

}
