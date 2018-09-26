
public class Main {
	
	
    public static int testLoop(int[] arr1) {
        int sum = 0;
				
		for (int i : arr1){
			sum += i;			
		}
		
        return sum;
    }

    public void $noinline$test() {
		int res = 0;
		
		final int[] arr1 = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
		final int[] arr2 = {13, 3, 45, 324, 32, 11, 643, 445, 23, 3};
	
		res += testLoop(arr1);
	
		try {
			if (testLoop(arr2) > 0)
				throw new Exception("expected");
		} catch (Exception e) {
			res /= testLoop(arr1);
		} finally {
			res -= testLoop(arr2);
		}
		
		res *= testLoop(arr1);
	
		System.out.println(res);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}


