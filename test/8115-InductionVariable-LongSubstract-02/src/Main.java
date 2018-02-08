
/**
 * Simple BIV test with different increments: substract-int/2addr.
 */
public class Main {
    
    static long threshold = 10;

    public void $noinline$test() {
        long i = 21;
		long j = 999;
		long m = 3;
		while ( i > threshold ) {
        i -= m;
        j -= 100 - m;
		}
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
