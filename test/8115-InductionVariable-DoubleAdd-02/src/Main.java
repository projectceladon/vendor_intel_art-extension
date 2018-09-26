
/**
 * Simple BIV test with different increments.
 */
public class Main {
    
    static double threshold = 10;

    public void $noinline$test() {
        double i = 0;
		double j = 2;
		double k = 5;
		double m = 32;
		double n = 7;
		while ( i < threshold ) {
            i += 2;
			j += m;
			k += n + 22;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
