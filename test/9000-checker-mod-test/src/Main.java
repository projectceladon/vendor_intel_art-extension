public class Main {
  static final int num = 16;
  static int[] a = new int[128];

  /// CHECK-START: int Main.TestMod(int) instruction_simplifier (before)
  /// CHECK-DAG: <<Par:i\d+>> ParameterValue
  /// CHECK-DAG: <<Num:i\d+>> IntConstant 4
  /// CHECK-DAG: <<Rem:i\d+>> Rem [<<Par>>,<<Num>>]
  /// CHECK-DAG:              Return [<<Rem>>]

  /// CHECK-START: int Main.TestMod(int) instruction_simplifier (after)
  /// CHECK-DAG: <<Par:i\d+>>  ParameterValue
  /// CHECK-DAG: <<Num:i\d+>>  IntConstant 3
  /// CHECK-DAG: <<Zero:i\d+>> IntConstant 0
  /// CHECK-NOT:               Rem
  /// CHECK-DAG: <<And:i\d+>>  And [<<Par>>,<<Num>>]
  /// CHECK-DAG: <<Geq:z\d+>>  GreaterThanOrEqual [<<Par>>,<<Zero>>]
  /// CHECK-DAG:               If [<<Geq>>]
  /// CHECK-DAG: <<Neq:z\d+>>  NotEqual [<<And>>,<<Zero>>]
  /// CHECK-DAG:               If [<<Neq>>]
  /// CHECK-DAG: <<Not:i\d+>>  Not [<<Num>>]
  /// CHECK-DAG: <<Or:i\d+>>   Or [<<And>>,<<Not>>]
  /// CHECK-DAG: <<Phi:i\d+>>  Phi [<<Zero>>,<<Or>>,<<And>>]
  /// CHECK-DAG:               Return [<<Phi>>]

  public static int TestMod(int paramInt) {
   return paramInt%4;
  }

  public static int TestModOne(int paramInt) {
   return paramInt%1;
  }

  public static int TestModTwo(int paramInt) {
    return paramInt%2;
  }

  /// CHECK-START: int Main.TestModPhi() instruction_simplifier (before)
  /// CHECK-DAG: <<Num:i\d+>> IntConstant 16 loop:none
  /// CHECK-DAG:              Phi                   loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<Mul:i\d+>> Mul loop:<<Loop>>
  /// CHECK-DAG: <<Rem:i\d+>> Rem [<<Mul>>,<<Num>>] loop:<<Loop>>

  /// CHECK-START: int Main.TestModPhi() instruction_simplifier (after)
  /// CHECK-DAG: <<Zero:i\d+>> IntConstant 0                         loop:none
  /// CHECK-DAG: <<Num:i\d+>>  IntConstant 15                        loop:none
  /// CHECK-DAG: <<Mul:i\d+>>  Mul                                   loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<And:i\d+>>  And [<<Mul>>,<<Num>>]                 loop:<<Loop>>
  /// CHECK-DAG: <<Geq:z\d+>>  GreaterThanOrEqual [<<Mul>>,<<Zero>>] loop:<<Loop>>
  /// CHECK-DAG:               If [<<Geq>>]                          loop:<<Loop>>
  /// CHECK-DAG: <<Neq:z\d+>>  NotEqual [<<And>>,<<Zero>>]           loop:<<Loop>>
  /// CHECK-DAG:               If [<<Neq>>]                          loop:<<Loop>>
  /// CHECK-DAG: <<Not:i\d+>>  Not [<<Num>>]                         loop:<<Loop>>
  /// CHECK-DAG: <<Or:i\d+>>   Or [<<And>>,<<Not>>]                  loop:<<Loop>>
  /// CHECK-DAG:               Phi [<<Zero>>,<<Or>>,<<And>>]         loop:<<Loop>>
  /// CHECK-NOT:               Rem                                   loop:<<Loop>>

  public static int TestModPhi() {
   int j = 0;
   for (int i = 0; i < 10; i++) {
     j = (j * i ) % num;
   }
  return j;
  }

  /// CHECK-START: int Main.TestModNegPhi() instruction_simplifier (before)
  /// CHECK-DAG: <<Num:i\d+>> IntConstant 16 loop:none
  /// CHECK-DAG:              Phi                   loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<Mul:i\d+>> Mul                   loop:<<Loop>>
  /// CHECK-DAG:              Rem [<<Mul>>,<<Num>>] loop:<<Loop>>

  /// CHECK-START: int Main.TestModNegPhi() instruction_simplifier (after)
  /// CHECK-DAG: <<Zero:i\d+>> IntConstant 0                         loop:none
  /// CHECK-DAG: <<Num:i\d+>>  IntConstant 15                        loop:none
  /// CHECK-DAG: <<Mul:i\d+>>  Mul                                   loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<And:i\d+>>  And [<<Mul>>,<<Num>>]                 loop:<<Loop>>
  /// CHECK-DAG: <<Geq:z\d+>>  GreaterThanOrEqual [<<Mul>>,<<Zero>>] loop:<<Loop>>
  /// CHECK-DAG:               If [<<Geq>>]                          loop:<<Loop>>
  /// CHECK-DAG: <<Neq:z\d+>>  NotEqual [<<And>>,<<Zero>>]           loop:<<Loop>>
  /// CHECK-DAG:               If [<<Neq>>]                          loop:<<Loop>>
  /// CHECK-DAG: <<Not:i\d+>>  Not [<<Num>>]                         loop:<<Loop>>
  /// CHECK-DAG: <<Or:i\d+>>   Or [<<And>>,<<Not>>]                  loop:<<Loop>>
  /// CHECK-DAG:               Phi [<<Zero>>,<<Or>>,<<And>>]         loop:<<Loop>>
  /// CHECK-NOT:               Rem                                   loop:<<Loop>>

  public static int TestModNegPhi() {
    int j = 0;
    for (int i = -128; i < 128; i++ ) {
       j = i * i % num;
    }
    return j;
  }

  public static void initArray(int[] a) {
    for (int i = 0; i < a.length; i++) {
     a[i] = i%4;
    }
  }

  /// CHECK-START: void Main.TestModIf(int[]) instruction_simplifier (before)
  /// CHECK-DAG:   <<Int:i\d+>> IntConstant 2
  /// CHECK-DAG:   <<Add:i\d+>> Add                   loop:<<Loop:B\d+>>
  /// CHECK-DAG:                Rem [<<Add>>,<<Int>>] loop:<<Loop>>

  /// CHECK-START: void Main.TestModIf(int[]) instruction_simplifier (after)
  /// CHECK-DAG:   <<Zero:i\d+>> IntConstant 0                         loop:none
  /// CHECK-DAG:   <<One:i\d+>>  IntConstant 1                         loop:none
  /// CHECK-DAG:   <<Add:i\d+>>  Add                                   loop:<<Loop:B\d+>>
  /// CHECK-DAG:   <<And:i\d+>>  And [<<Add>>,<<One>>]                 loop:<<Loop>>
  /// CHECK-DAG:   <<Geq:z\d+>>  GreaterThanOrEqual [<<Add>>,<<Zero>>] loop:<<Loop>>
  /// CHECK-DAG:                 If [<<Geq>>]                          loop:<<Loop>>
  /// CHECK-DAG:   <<Neq:z\d+>>  NotEqual [<<And>>,<<Zero>>]           loop:<<Loop>>
  /// CHECK-DAG:                 If [<<Neq>>]                          loop:<<Loop>>
  /// CHECK-DAG:   <<Not:i\d+>>  Not [<<One>>]                         loop:<<Loop>>
  /// CHECK-DAG:   <<Or:i\d+>>   Or [<<And>>,<<Not>>]                  loop:<<Loop>>
  /// CHECK-DAG:                 Phi [<<Zero>>,<<Or>>,<<And>>]         loop:<<Loop>>
  public static void TestModIf(int[] a) {
    int i = 0;
    int s = 128;
    while (i++ < a.length - 2) {
      if (i % 2 == 0) {
        a[i] = s++;
      }
    }
  }

 //Negetive TestCase
 /// CHECK-START: void Main.TestModSwitch(int[]) instruction_simplifier (before)
 /// CHECK-DAG: <<Num:i\d+>> IntConstant 5         loop:none
 /// CHECK-DAG: <<Phi:i\d+>> Phi                   loop:<<Loop:B\d+>>
 /// CHECK-DAG:              Rem [<<Phi>>,<<Num>>] loop:<<Loop>>

 /// CHECK-START: void Main.TestModSwitch(int[]) instruction_simplifier (after)
 /// CHECK-DAG: <<Num:i\d+>> IntConstant 5         loop:none
 /// CHECK-DAG: <<Phi:i\d+>> Phi                   loop:<<Loop:B\d+>>
 /// CHECK-DAG:              Rem [<<Phi>>,<<Num>>] loop:<<Loop>>
 public static void TestModSwitch(int[] a) {
  for (int i = 0; i < a.length; i++) {
    switch (i%5) {
     case 3:
      a[i]--;
      break;
     default:
     break;
    }
  }
 }

  public static int arraySum(int [] a) {
    int sum = 0;
    for (int i =0; i < a.length; i++) {
      sum += a[i];
    }
    return sum;
  }

  public static void expectEquals(int expected, int result) {
    if (result != expected) {
      throw new Error("Expected: " + expected + ", found: " + result);
    }
  }

  public static void main(String[] args) {
    expectEquals(-1, TestMod(-5));
    expectEquals(0, TestModOne(-100));
    expectEquals(2, TestMod(10));
    expectEquals(0, TestModPhi());
    initArray(a);
    TestModIf(a);
    expectEquals(10145, arraySum(a));
    expectEquals(1, TestModNegPhi());
    TestModSwitch(a);
  }

}
