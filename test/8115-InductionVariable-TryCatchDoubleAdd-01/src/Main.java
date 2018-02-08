
/**
 * myLoop will be inlined into test().
 * so it's expected that "IV.*loop increment: 1" ... "IV.*loop increment: 5" will be found in logcat
 */
 
public class Main {

	static double threshold = 10.4;

	final double myLoop(double step) {
		double i = 0;
		
		while (i < threshold) {
			i += step;
		}
		
		return i;
	}

	public void $noinline$test() {	
		double res = 7.2;
	
		res += myLoop(1.1); // Before try-catch.
	
		try {
			if (myLoop(2.2) > 0)
				throw new Exception("expected"); // In Try section
		} catch (Exception e) {
			res /= myLoop(3.3); // In catch secton.
		} finally {
			res -= myLoop(4.4); // in Finally section.
		}
		
		res *= myLoop(5.5); // After try-catch.
	
		System.out.println(res);
	}
	

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}