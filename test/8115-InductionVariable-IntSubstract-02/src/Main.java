
/**
 * Simple BIV test with different increments: add-int/2addr .
 */
public class Main {

	static int threshold = 10;

	public void $noinline$test() {
		int i = 30;
		int j = 121;
        int k = 49;
		int m = 9;
		int m1 = m;
		while ( i > threshold ) {
			int m2 = m - 1;
			i -= m;
			j -= m1;
			k -= m2;
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
