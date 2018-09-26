/**
 * Test BIV with multiple loops.
 */
public class Main {

	static int iterations = 10;

	public void $noinline$test() {

		int m = 0;
		for ( int i = 0; i < iterations; i++) {
           m = m + 2;
		}
		
		for ( int j = 0; j < iterations; j += 2) {
			m = m + 1;
		}
		
			System.out.println(m);
	}
	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
