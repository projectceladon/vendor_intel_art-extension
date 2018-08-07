
/**
 * Simple BIV test: add-double/2addr.
 */
public class Main {
    
    static double threshold = 10;

	/**
	 * Simple test add-double/2addr 
	 */
	public void $noinline$test() {
		double i = 0;
		while ( i < threshold ) {
			i = i + 2.234;
		}
        System.out.println(i);
    }

	public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
