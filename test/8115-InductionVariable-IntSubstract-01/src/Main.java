
/**
 * Simple BIV test : substract-int/2addr.
 */
public class Main {
    
    static int threshold = 10;

    public void $noinline$test() {
        int i = 28;
		int j = 87;
		int m = 3;
		while ( i > threshold ) {
        i -= m;
        j -= m - 1;
		}
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
