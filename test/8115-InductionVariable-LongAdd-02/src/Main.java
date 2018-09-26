
/**
 * Simple BIV test with different increments.
 */
public class Main {
    
    static long threshold = 10;

    public void $noinline$test() {
        long i = 0;
		long j = 2;
		long k = 5;
		long m = 382;
		long n = 293;
		while ( i < threshold ) {
            i += 2;
			j += m;
			k += n + 92;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
