/**
 * Test BIV that in control flow branch: IfElse.
 */
public class Main {

	static int iterations = 5;

	public void $noinline$test() {
		int i = 0;	    
		int j = 2;
		while (i < iterations) {
	
			if (j < 5) {
               i = i + 1;
			} else if ( i < 10 ) {
				i = 5*j - 2;
			} else if ( i < 20 ) {
				i = j*(i + 3);
			} else if ( i < 30 ) {
				i = i * j - 5;
			} else {
                i = j * (i + 2) ;
			}
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
