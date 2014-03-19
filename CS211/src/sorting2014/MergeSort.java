package sorting2014;

/**
 * Merge Sort.
 * @author Evdzhan Mustafa enm3@aber.ac.uk
 *
 */
public class MergeSort implements Sorter {
	private Comparable[] primaryArr;
	private Comparable[] secondaryArr;

	@SuppressWarnings("unchecked")
	public void sort(Comparable[] items, int cutoff) {

		this.primaryArr = items;

		secondaryArr = new Comparable[items.length];

		mergeSort(0, items.length - 1);

	}

	/**
	 * Merge sort - works by continually splitting the array in halves. Split in
	 * halves until arrays of size 1 is left. This is achieved by recursive
	 * calls. After both arrays are of size split , they are sorted and then
	 * rearranged into, the primary array.
	 * 
	 * @param first
	 *            the index of the first element in the (sub)array
	 * @param last
	 *            the index of the last element in the (sub)array
	 */
	private void mergeSort(int first, int last) {

		if (first < last) { // i.e. split only if the array size is > 1

			int mean = first + ((last - first) / 2); // find the index of the
														// middle element

			mergeSort(first, mean); // recursively split the left (sub)array

			mergeSort(mean + 1, last); // recursively split the right (sub)array

			merge(first, mean, last); // merge the arrays in sorted manner

		}

	}

	/**
	 * Merges two sub arrays, by putting them into the primaryArr in sorted
	 * manner. That is : 1) compare the elements, indexed as next, from both
	 * subarrays 2) take the lower one and place it into the primary array,
	 * increment the next index by 1 3) repeat
	 * 
	 * Repeat this until both sub arrays are placed onto the primarryArr.
	 * 
	 * @param first
	 *            index of the first element
	 * @param mean
	 *            index of the middle element
	 * @param last
	 *            index of the last element
	 */
	private void merge(int first, int mean, int last) {

		for (int i = first; i <= last; i++) { // copy the current sub arrays
			secondaryArr[i] = primaryArr[i];
		}

		int arr_one = first;
		int u = mean + 1;
		int arr_two = first;

		while (arr_one <= mean && u <= last) {

			if (secondaryArr[arr_one].compareTo(secondaryArr[u]) <= 0) { // compare
				primaryArr[arr_two] = secondaryArr[arr_one];
				arr_one++;
			} else {
				primaryArr[arr_two] = secondaryArr[u];
				u++;

			}
			arr_two++;

		}
		while (arr_one <= mean) {
			primaryArr[arr_two] = secondaryArr[arr_one];
			arr_one++;
			arr_two++;
		}

	}

}