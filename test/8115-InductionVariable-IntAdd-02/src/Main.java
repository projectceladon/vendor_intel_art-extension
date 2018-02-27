
/**
 * Simple BIV test with different increments.
 */
public class Main {

	static int threshold = 10;

	public void $noinline$test() {
		int i = 0;
		int j = 9;
        int k = 13;
		int m = 3;
		int m1 = m;
		while ( i < threshold ) {
			int m2 = m + 5;
			i += m;
			j += m1;
			k += m2;
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
