
/**
 * Commulate the operands.
 */
public class Main {
    
    static int threshold = 50;

    public void test() {
        int i0 = 0;
		int i1 = 274;
		int i2 = 762;
		int i3 = 53;
		int i4 = 87;
		int i5 = 123;
		int m = 15;
		while ( i0 < threshold ) {
			i0 = 5 + i0;
            i1 = m + i1;
			i2 = m + 3 + i2;
			i3 = 74 + m + i3;
			i4 = i4 + 74 + m;
			i5 = 74 + i5 + m;
		}
        System.out.println(i0);
    }

	public static void main(String[] args) {
		new Main().test();
    }
}
