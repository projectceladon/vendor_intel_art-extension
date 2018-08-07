
/**
 * Test the mix of different BIVs.
 */
public class Main {

	static int iterations = 5;

	public void $noinline$test() {
		int i1 = 0;
		int i2 = 83442;
		int m = 391;
		long j1 = -142l;
		long j2 = 783l;
		float f = 23.4632f;
		double d = 0;

		while (i1 < iterations) {
			i1 = i1 + 1;	
			i2 = i2 - m;	
			j1 = j1 + 13l;
			j2 = j2 - 57l;
			f = f + 349f;
			d = d + 97;
		}
		System.out.println(i1);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
