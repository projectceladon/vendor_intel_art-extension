
/**
 * Simple BIV test with different increments.
 */
public class Main {
    
    static float threshold = 10f;

    public void $noinline$test() {
        float i = 0;
		float j = 1.332f;
		float k = 3.883f;
		float m = 1.43f;
		float n = 5;
		while ( i < threshold ) {
            i += 2;
			j += m;
			k += n + 23.3552f;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
