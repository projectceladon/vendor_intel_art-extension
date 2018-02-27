
/**
 * Simple BIV test : add-int/2addr.
 */
public class Main {

	static int threshold = 10;

	public void $noinline$test() {
		int i = 0;
		int j = 5;
		int m = 3;
		while ( i < threshold ) {
			i += m;
			j += m + 7;
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
