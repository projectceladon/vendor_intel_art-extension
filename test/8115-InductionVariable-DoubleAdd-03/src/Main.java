
/**
 * Simple BIV test with different increments.
 */
public class Main {
    
    static double threshold = 10;

    public void $noinline$test() {
        double i = 0;
		double j = 39;
		double k = 17;
		double m = 2;
		double m1 = m;
		while ( i < threshold ) {
			double m2 = m + 1;
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
