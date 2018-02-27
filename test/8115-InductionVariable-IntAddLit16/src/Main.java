
/**
 * Simple BIV test: add-int/lit8 add-int/lit16.
 */
public class Main {

	static int threshold = 10;

	public void $noinline$test() {
		int i = 0;
		int j = 0;
		while ( i < threshold ) {
			i = i + 2;
			j = j + 182;
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
