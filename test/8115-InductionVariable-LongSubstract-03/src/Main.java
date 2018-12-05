
/**
 * Simple BIV test with differents increments: add-int/2addr.
 */
public class Main {

	static long threshold = 10;

	public void $noinline$test() {
		long i = 30;
		long j = 421;
        long k = 319;
		long m = 9;
		long m1 = m + 36;
		while ( i > threshold ) {
			long m2 = m + 54;
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
