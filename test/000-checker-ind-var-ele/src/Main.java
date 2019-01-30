import java.util.*;

public class Main{

  static int n = 10;

  public static void assertIntEquals(int expected, int result) {
    if (expected != result) {
      throw new Error("Expected: " + expected + ", found: " + result);
    }
  }

  public static void assertFloatEquals(float expected, float result) {
    if (Float.compare(expected,result)!=0) {
      throw new Error("Expected: " + expected + ", found: " + result);
    }
  }
  public static void assertDoubleEquals(double expected, double result) {
    if (Double.compare(expected,result)!=0) {
      throw new Error("Expected: " + expected + ", found: " + result);
    }
  }

/// CHECK-START: void Main.Test1() loop_optimization (before)
/// CHECK-DAG: <<Inc:i\d+>> IntConstant 2 loop:none
/// CHECK-DAG: <<Phi:i\d+>> Phi loop:<<Loop:B\d+>>
/// CHECK-DAG: Mul loop:<<Loop>>
/// CHECK-DAG: Add loop:<<Loop>>
/// CHECK-DAG: Add loop:<<Loop>>
/// CHECK-DAG: Add [<<Phi>>,<<Inc>>] loop:<<Loop>>

//
/// CHECK-START: void Main.Test1() loop_optimization (after)
/// CHECK-DAG: <<Inc1:i\d+>> IntConstant 2 loop:none
/// CHECK-DAG: <<Inc2:i\d+>> IntConstant 12 loop:none
/// CHECK-DAG:               Mul          loop:none
/// CHECK-DAG:               Add          loop:none
/// CHECK-DAG:               Mul          loop:none
/// CHECK-DAG:               Add          loop:none
/// CHECK-DAG: <<Biv:i\d+>>  Phi        loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi1:i\d+>> Phi        loop:<<Loop>>
/// CHECK-DAG: <<Phi2:i\d+>> Phi        loop:<<Loop>>
/// CHECK-DAG:               Mul        loop:<<Loop>>
/// CHECK-DAG:               Add        loop:<<Loop>>
/// CHECK-DAG:               Add        loop:<<Loop>>
/// CHECK-DAG:               Add [<<Biv>>,<<Inc1>>]      loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi1>>,<<Inc2>>]      loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi2>>,<<Inc2>>]      loop:<<Loop>>

/// CHECK-START: void Main.Test1() dead_code_elimination$final (after)
/// CHECK-NOT:               Mul         loop:<<B\d+>>
/// CHECK-NOT:               Mul         loop:none
public static void Test1() {
  int arr[] = new int[50];
  int k =0; int j=0;
  while (k<n) {
    j = 6*k+1;
   arr[j] = arr[j]-2;
   k = k+2;
  }
}

/// CHECK-START: int Main.Test2() loop_optimization (before)
/// CHECK-DAG: <<Inc:i\d+>> IntConstant 1 loop:none
/// CHECK-DAG: <<Phi1:i\d+>> Phi loop:<<Loop:B5>>
/// CHECK-DAG: <<Phi2:i\d+>> Phi loop:<<Loop>>
/// CHECK-DAG: Mul loop:<<Loop>>
/// CHECK-DAG: Add loop:<<Loop>>
/// CHECK-DAG: Add loop:<<Loop>>
/// CHECK-DAG: Add loop:<<Loop>>
/// CHECK-DAG: Add [<<Phi1>>,<<Inc>>] loop:<<Loop>>

//
/// CHECK-START: int Main.Test2() loop_optimization (after)
/// CHECK-DAG: <<Inc1:i\d+>> IntConstant 1 loop:none
/// CHECK-DAG: <<Inc2:i\d+>> IntConstant 11 loop:none
/// CHECK-DAG:               Mul            loop:none
/// CHECK-DAG:               Add            loop:none
/// CHECK-DAG:               Mul            loop:none
/// CHECK-DAG:               Add            loop:none
/// CHECK-DAG: <<Phi1:i\d+>> Phi        loop:<<Loop:B5>>
/// CHECK-DAG: <<Phi2:i\d+>> Phi        loop:<<Loop>>
/// CHECK-DAG: <<Phi3:i\d+>> Phi        loop:<<Loop>>
/// CHECK-DAG: <<Phi4:i\d+>> Phi        loop:<<Loop>>
/// CHECK-DAG:               Mul        loop:<<Loop>>
/// CHECK-DAG:               Add        loop:<<Loop>>
/// CHECK-DAG:               Add        loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi1>>,<<Inc1>>]      loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi2>>,<<Inc2>>]      loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi3>>,<<Inc2>>]      loop:<<Loop>>

/// CHECK-START: int Main.Test2() dead_code_elimination$final (after)
/// CHECK-NOT:               Mul            loop:none
/// CHECK-NOT:               Mul         loop:<<B\d+>>
public static int Test2() {
 int n = 36;
 float [] radius = new float [n];
Random r = new Random();
for (int i = 0; i< n ; i++) {
  radius[i] = r.nextFloat();
}
int sum = 0;
for (int ii= 0 ; ii< n; ii++){
  int k = 11*ii;
  int m = k + 5;
  sum += radius[ii] + m;
}
 return sum;
}

/// CHECK-START: int Main.Test3() loop_optimization (before)
/// CHECK-DAG: <<Inc:i\d+>> IntConstant 1 loop:none
/// CHECK-DAG: <<Phi1:i\d+>> Phi          loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi2:i\d+>> Phi          loop:<<Loop>>
/// CHECK-DAG: Shl                        loop:<<Loop>>
/// CHECK-DAG: Mul                        loop:<<Loop>>
/// CHECK-DAG: Add [<<Phi1>>,<<Inc>>]     loop:<<Loop>>

//
/// CHECK-START: int Main.Test3() loop_optimization (after)
/// CHECK-DAG: <<Inc1:i\d+>> IntConstant 1           loop:none
/// CHECK-DAG: <<Inc2:i\d+>> IntConstant 6           loop:none
/// CHECK-DAG:               Mul                     loop:none
/// CHECK-DAG:               Add                     loop:none
/// CHECK-DAG: <<Phi1:i\d+>> Phi                     loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi2:i\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG: <<Phi3:i\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG:               Shl                     loop:<<Loop>>
/// CHECK-DAG:               Mul                     loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi1>>,<<Inc1>>] loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi2>>,<<Inc2>>] loop:<<Loop>>

/// CHECK-START: int Main.Test3() dead_code_elimination$final (after)
/// CHECK-NOT: Mul loop:<<B\d+>>
public static int Test3() {
 int k =0;
 for (int i= 0; i< n; i++){
   if (i%2 == 0) {
     k = (int) 6.14*i;
   }
   else {
    k = (int)2.69* i;
   }
 }
return k;
}

/// CHECK-START: float Main.Test4() loop_optimization (before)
/// CHECK-DAG: <<Inc:f\d+>> FloatConstant 1 loop:none
/// CHECK-DAG: <<Phi1:f\d+>> Phi          loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi2:f\d+>> Phi          loop:<<Loop>>
/// CHECK-DAG: Mul                        loop:<<Loop>>
/// CHECK-DAG: Add                        loop:<<Loop>>
/// CHECK-DAG: Add                        loop:<<Loop>>
/// CHECK-DAG: Add [<<Phi1>>,<<Inc>>]     loop:<<Loop>>

/// CHECK-START: float Main.Test4() loop_optimization (after)
/// CHECK-DAG: <<Inc1:f\d+>> FloatConstant 11.11     loop:none
/// CHECK-DAG: <<Inc2:f\d+>> FloatConstant 2.69      loop:none
/// CHECK-DAG: <<Inc3:f\d+>> FloatConstant 1         loop:none
/// CHECK-DAG:               Mul                     loop:none
/// CHECK-DAG:               Add                     loop:none
/// CHECK-DAG:               Mul                     loop:none
/// CHECK-DAG:               Add                     loop:none
/// CHECK-DAG: <<Phi1:f\d+>> Phi                     loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi2:f\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG: <<Phi3:f\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG: <<Phi4:f\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG: <<Phi5:i\d+>> Phi                     loop:<<Loop>>
/// CHECK-DAG:               Mul                     loop:<<Loop>>
/// CHECK-DAG:               Add                     loop:<<Loop>>
/// CHECK-DAG:               Add                     loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi1>>,<<Inc3>>] loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi2>>,<<Inc1>>] loop:<<Loop>>
/// CHECK-DAG:               Add [<<Phi3>>,<<Inc1>>] loop:<<Loop>>

/// CHECK-START: float Main.Test4() dead_code_elimination$final (after)
/// CHECK-NOT: Mul loop:<<B\d+>>

public static float Test4(){
  float k = 0.0f;
  int times = 0;
  for (float f = 0.0f ; f <= 10.0f; f += 1.0f) {
    k = 11.11f * f + 2.69f;
    times++;
   }
   return k*times;
 }

public static void TestSub1() {
  int arr[] = new int[50];
  int k =0; int j=0;
  while (k<n) {
    j = 6*k-1;
   arr[j] = arr[j]-2;
   k = k+2;
  }
}

/// CHECK-START: double Main.Test5() loop_optimization (before)
/// CHECK-DAG: <<Var1:d\d+>> DoubleConstant 0 loop:none
/// CHECK-DAG: <<Var2:d\d+>> DoubleConstant 1 loop:none
/// CHECK-DAG: <<Phi1:d\d+>> Phi [<<Var1>>,<<Add:d\d+>>]     loop:<<Loop:B\d+>>
/// CHECK-DAG: <<Phi2:d\d+>> Phi [<<Var1>>,<<Sub:d\d+>>]     loop:<<Loop>>
/// CHECK-DAG:               Mul                        loop:<<Loop>>
/// CHECK-DAG:               Mul                        loop:<<Loop>>
/// CHECK-DAG: <<Sub>>       Sub                        loop:<<Loop>>
/// CHECK-DAG: <<Add>>       Add [<<Phi1>>,<<Var2>>]    loop:<<Loop>>

/// CHECK-START: double Main.Test5() loop_optimization (after)
/// CHECK-DAG: <<Var1:d\d+>>    DoubleConstant 0           loop:none
/// CHECK-DAG: <<Var2:d\d+>>    DoubleConstant 1           loop:none
/// CHECK-DAG: <<Var3:d\d+>>    DoubleConstant 3.14        loop:none
/// CHECK-DAG:                  Mul                        loop:none
/// CHECK-DAG: <<Add2:d\d+>>    Add                        loop:none
/// CHECK-DAG: <<Phi1:d\d+>>    Phi [<<Var1>>,<<Add:d\d+>>]     loop:<<Loop:B\d+>>
/// CHECK-DAG: <<NewPhi:d\d+>>  Phi [<<Add2>>,<<NewAdd:d\d+>>]  loop:<<Loop>>
/// CHECK-DAG: <<Phi2:d\d+>>    Phi [<<Var1>>,<<Sub:d\d+>>]    loop:<<Loop>>
/// CHECK-DAG:                  Mul                        loop:<<Loop>>
/// CHECK-DAG:                  Mul                        loop:<<Loop>>
/// CHECK-DAG: <<Sub>>          Sub                        loop:<<Loop>>
/// CHECK-DAG: <<Add>>          Add [<<Phi1>>,<<Var2>>]    loop:<<Loop>>
/// CHECK-DAG: <<NewAdd>>      Add [<<NewPhi>>,<<Var3>>] loop:<<Loop>>

/// CHECK-START: double Main.Test5() dead_code_elimination$final (after)
/// CHECK:  Mul loop:<<Loop:B\d+>>
/// CHECK-NEXT: Sub loop:<<Loop>>

public static double Test5(){
  double volume = 0.0d;
  double pi = 3.14;
  double c = 4/3;
  int times = 0;
  for (double r = 0.0d ; r <= 10.0d; r += 1.0d) {
    volume = pi*c*r*r;
    volume -= 100d;
   }
   return volume;
 }

public static void main(String[] args) {

  Test1();
  assertIntEquals(7110, Test2());
  assertFloatEquals (18.0f, Test3());
  assertFloatEquals(1251.6901f,Test4());
  assertDoubleEquals(214.0,Test5());
}


}
