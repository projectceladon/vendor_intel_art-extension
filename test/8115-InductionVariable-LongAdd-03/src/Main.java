
/**
 * Simple BIV test with different increments.
 */
public class Main {
    
    static long threshold = 10;

    public void $noinline$test() {
        long i = 0;
		long j = 238;
		long k = 7;
		long m = 2;
		long m1 = m + 383;
		while ( i < threshold ) {
			long m2 = m * 74;
            i = i + m;
			j = j + m1;
			k = k + m2;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
