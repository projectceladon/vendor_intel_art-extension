
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
		float m1 = m;
		while ( i < threshold ) {
			float m2 = m + 1;
            i = i + m;
			j = j + m1;
			k = k + m2;
        }
        System.out.println(i);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}
