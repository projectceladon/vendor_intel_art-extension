public class Main {

public static void assertIntEquals(int expected, int result) {
  if (expected != result) {
    throw new Error("Expected: " + expected + ", found: " + result);
  }
}


/// CHECK-START-X86_64: int Main.and_not(int, int) instruction_simplifier_x86_64 (before)
/// CHECK-DAG:    Phi     loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-DAG:    Not     loop:<<Loop>>      outer_loop:none
/// CHECK-DAG:    And     loop:<<Loop>>      outer_loop:none

/// CHECK-START-X86_64: int Main.and_not(int, int) instruction_simplifier_x86_64 (after)
/// CHECK-DAG:      AndNot loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-NOT:      Not    loop:<<Loop>>      outer_loop:none
/// CHECK-NOT:      And    loop:<<Loop>>      outer_loop:none

public static int and_not( int x, int y) {
  int j = 1;
  int k = 2;
  for (int i = 0 ; i < 64; i++ ) {
   x = x & ~i;
   y = y | i;
  }
  return x & ~y;
}

/// CHECK-START-X86_64: int Main.reset_lowest_set_bit(int) instruction_simplifier_x86_64 (before)
/// CHECK-DAG:    Phi     loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-DAG:    Add     loop:<<Loop>>      outer_loop:none
/// CHECK-DAG:    And     loop:<<Loop>>      outer_loop:none

/// CHECK-START-X86_64: int Main.reset_lowest_set_bit(int) instruction_simplifier_x86_64 (after)
/// CHECK-DAG:      BitwiseAddRight loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-NOT:      And    loop:<<Loop>>               outer_loopLnone

public static int reset_lowest_set_bit(int x) {
  int y = x;
  int j = 5;
  int k = 10;
  int l = 20;
  for(int i = -64 ; i < 64; i++){
  y = i & i-1;
  j += y;
  j = j & j-1;
  k +=j;
  k = k & k-1;
  l +=k;
  l = l & l-1;
  }
  return y + j + k + l;
}

/// CHECK-START-X86_64: int Main.get_mask_lowest_set_bit(int) instruction_simplifier_x86_64 (before)
/// CHECK-DAG:    Add     loop:none    
/// CHECK-DAG:    Xor     loop:none    

/// CHECK-START-X86_64: int Main.get_mask_lowest_set_bit(int) instruction_simplifier_x86_64 (after)
/// CHECK-DAG:      BitwiseAddRight  loop:none
/// CHECK-NOT:      Add    loop:none
/// CHECK-NOT:      Xor    loop:none

public static int get_mask_lowest_set_bit(int x) {
  return (x-1) ^ x;
}

/// CHECK-START-X86_64: int Main.get_lowest_set_bit(int) instruction_simplifier_x86_64 (before)
/// CHECK-DAG:    Phi     loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-DAG:    Neg     loop:<<Loop>>      outer_loop:none
/// CHECK-DAG:    And     loop:<<Loop>>      outer_loop:none

/// CHECK-START-X86_64: int Main.get_lowest_set_bit(int) instruction_simplifier_x86_64 (after)
/// CHECK-DAG:      AndNeg loop:<<Loop:B\d+>> outer_loop:none
/// CHECK-NOT:      Neg    loop:<<Loop>>      outer_loop:none
/// CHECK-NOT:      And    loop:<<Loop>>      outer_loop:none

public static int get_lowest_set_bit(int x) {
  int y = x;
  int j = 6;
  int k = 7;
  int l = 8;
  int m = 9;
  for(int i = 0; i< 128; i++){
   y = i & -i;
   j += y;
   j = j & -j;
   k +=j;
   k = k & -k;
   l +=k;
   l = l & -l;
   m -=l;
   m = m & -m;
  }
  return y + j + k + l + m;
}
public static void main(String[] args){
  int x = 50;
  int y = x/2;
  assertIntEquals(0, and_not(x,y));
  assertIntEquals(-20502606, reset_lowest_set_bit(x));
  assertIntEquals(7, get_lowest_set_bit(x));
  assertIntEquals(1, get_mask_lowest_set_bit(y));

}

}
