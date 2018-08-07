
/**
 * myLoop will be inlined into test().
 * so it's expected that "IV.*loop increment: 1" ... "IV.*loop increment: 5" will be found in logcat
 */
 
public class Main {

	static int threshold = 10;

	final int myLoop(int step) {
		int i = 0;
		
		while (i < threshold) {
			i += step;			
			
			int j = 0;
			while (j < threshold)
			{
				j += step * 6;
			}
		}
		
		return i;
	}

	public void $noinline$test() {	
		int res = 7;
	
		res += myLoop(1); // Before try-catch.
	
		try {
			if (myLoop(2) > 0)
				throw new Exception("expected"); // In Try section
		} catch (Exception e) {
			res ^= myLoop(3); // In catch secton.
		} finally {
			res -= myLoop(4); // in Finally section.
		}
		
		res *= myLoop(5); // After try-catch.
	
		System.out.println(res);
	}
	

	public static void main(String[] args) {
		new Main().$noinline$test();
	}
}