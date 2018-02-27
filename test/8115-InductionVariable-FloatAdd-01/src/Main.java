
/**
 * Simple BIV test : add-float/2addr.
 */
public class Main {
    
    static float threshold = 10f;

    public void $noinline$test() {
        float i = 0;
		while ( i < threshold ) {
            i = i + 2.87f;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
