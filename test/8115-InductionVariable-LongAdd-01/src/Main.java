
/**
 * Simple BIV test: add-long/2addr.
 */
public class Main {
    
    static long threshold = 10;

    public void $noinline$test() {
        long j = 0;
		while ( j < threshold ) {
			j = j + 2;
		}
        System.out.println(j);
    }

	public static void main(String[] args) {
		new Main().$noinline$test();
    }
}
