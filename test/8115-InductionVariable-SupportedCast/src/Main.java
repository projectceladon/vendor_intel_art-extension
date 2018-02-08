
/**
 * Test supported case: int_to_short and int_to_byte.
 */
public class Main {
    
    static int threshold = 10;

    public void $noinline$test() {
        int i = 0;
        int j = 3;
		int m = 45;
		int n = 5;
		while ( i < threshold ) {
		i = i + (short)j;
	    m = m - (byte)n;	
		}
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
