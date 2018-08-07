
/**
 * myLoop will be inlined into test().
 * so it's expected that "IV.*loop increment: 1" ... "IV.*loop increment: 5" will be found in logcat
 */
 
public class Main {

	static float threshold = 10.4f;

	final float myLoop(float step) {
		float i = 0.0f;
		
		while (i < threshold) {
			i += step;
		}
		
		return i;
	}

	public void $noinline$test() {	
		float res = 7.2f;
	
		res += myLoop(1.1f); // Before try-catch.
	
		try {
			if (myLoop(2.2f) > 0)
				throw new Exception("expected"); // In Try section
		} catch (Exception e) {
			res /= myLoop(3.3f); // In catch secton.
		} finally {
			res -= myLoop(4.4f); // in Finally section.
		}
		
		res *= myLoop(5.5f); // After try-catch.
	
		System.out.println(res);
	}
	

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}