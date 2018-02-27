
/**
 * Test mix BIVs with different constant type convert.
 */
public class Main {

	static int iterations = 5;

	public void $noinline$test() {
		int i = 0;
		long l1 = 332l;
		float f1 = 0.234f;
		float f2 = 83.982f;
		double d1 = -242;
		double d2 = 475;
		while (i < iterations) {
			i = i + 1;	
			l1 = l1 + 16;
			f1 = f1 + 749;
			f2 = f2 + 39l;
			d1 = d1 + 84l;
			d2 = d2 + 97.323f;
		}
		System.out.println(i);
	}

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}
