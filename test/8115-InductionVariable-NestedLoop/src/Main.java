/**
 * Test BIV within nested loops.
 */
public class Main {

	static int iterations = 10;

	public void $noinline$test() {

		int m = 0;	
		int n = 0;	

		for ( int i = 0 ; i < iterations; i++) {

			m = m + 1;

			for ( int j = 1; j < iterations; j+=2) {

				n = n + 1;

			}
		}
		System.out.println(m);
	}
	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
