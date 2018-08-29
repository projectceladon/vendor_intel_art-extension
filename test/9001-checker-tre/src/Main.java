/*
 * checker test case for tail-recursion-elimination optimization
 */
public class Main
{
  public Main() {}
  
  public static void assertIntEquals(int expected, int result) {
    if (expected != result) {
      throw new Error("Expected: " + expected + ", found: " + result);
    }
  }


  /// CHECK-START: int Main.arithmeticSeries1(int) TRE (before)
  /// CHECK-DAG: <<Par:i\d+>>   ParameterValue             loop:none
  /// CHECK-DAG: <<Val:i\d+>>   IntConstant 0              loop:none
  /// CHECK-DAG: <<Val1:i\d+>>  IntConstant -1             loop:none
  /// CHECK-DAG:                NotEqual [<<Par>>,<<Val>>] loop:none
  /// CHECK-DAG:                Add [<<Par>>,<<Val1>>]     loop:none
  /// CHECK-DAG:                InvokeVirtual method_name:Main.arithmeticSeries1 loop:none

  /// CHECK-START: int Main.arithmeticSeries1(int) TRE (after)
  /// CHECK-DAG: <<Par:i\d+>>   ParameterValue              loop:none
  /// CHECK-DAG: <<Val:i\d+>>   IntConstant 0               loop:none
  /// CHECK-DAG: <<Val1:i\d+>>  IntConstant -1              loop:none
  /// CHECK-DAG: <<PhiA:i\d+>>  Phi [<<Val>>,<<AddA:i\d+>>] loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<PhiN:i\d+>>  Phi [<<Par>>,<<AddN:i\d+>>] loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:                SuspendCheck                loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:                NotEqual [<<PhiN>>,<<Val>>] loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddN>>       Add [<<PhiN>>,<<Val1>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddA>>       Add [<<PhiA>>,<<PhiN>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddR:i\d+>>  Add [<<PhiA>>,<<Val>>]      loop:none
  /// CHECK-DAG:                Return [<<AddR>>]           loop:none
  /// CHECK-NOT:                InvokeVirtual method_name:Main.arithmeticSeries1

  public int arithmeticSeries1(int n)
  {
    if (n == 0) {
      return 0;
    }

    return n + arithmeticSeries1(n - 1);
  }
  
  /// CHECK-START: int Main.arithmeticSeries2(int, int) TRE (before)
  /// CHECK-DAG: <<ParN:i\d+>> ParameterValue              loop:none
  /// CHECK-DAG: <<ParA:i\d+>> ParameterValue              loop:none
  /// CHECK-DAG: <<Val:i\d+>>  IntConstant 0               loop:none
  /// CHECK-DAG: <<Val1:i\d+>> IntConstant -1              loop:none
  /// CHECK-DAG:               NotEqual [<<ParN>>,<<Val>>] loop:none
  /// CHECK-DAG:               Add [<<ParA>>,<<ParN>>]     loop:none
  /// CHECK-DAG:               Add [<<ParN>>,<<Val1>>]     loop:none
  /// CHECK-DAG:               InvokeVirtual method_name:Main.arithmeticSeries2 loop:none

  /// CHECK-START: int Main.arithmeticSeries2(int, int) TRE (after)
  /// CHECK-DAG: <<ParN:i\d+>> ParameterValue               loop:none
  /// CHECK-DAG: <<ParA:i\d+>> ParameterValue               loop:none
  /// CHECK-DAG: <<Val:i\d+>>  IntConstant 0                loop:none
  /// CHECK-DAG: <<Val1:i\d+>> IntConstant -1               loop:none
  /// CHECK-DAG: <<PhiA:i\d+>> Phi [<<ParA>>,<<AddA:i\d+>>] loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<PhiN:i\d+>> Phi [<<ParN>>,<<AddN:i\d+>>] loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:               SuspendCheck                 loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:               NotEqual [<<PhiN>>,<<Val>>]  loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddA>>      Add [<<PhiA>>,<<PhiN>>]      loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddN>>      Add [<<PhiN>>,<<Val1>>]      loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:               Return [<<PhiA>>]            loop:none
  /// CHECK-NOT:               InvokeVirtual method_name:Main.arithmeticSeries2

  public int arithmeticSeries2(int n, int acc)
  {
    if (n == 0) {
      return acc;
    }

    acc += n; 

    return arithmeticSeries2(n - 1, acc);
  }

 
  /// CHECK-START: int Main.notInlineableSeries(int) TRE (before)
  /// CHECK-DAG: <<ParN:i\d+>> ParameterValue              loop:none
  /// CHECK-DAG: <<Val:i\d+>>  IntConstant 0               loop:none
  /// CHECK-DAG: <<Val1:i\d+>> IntConstant -1              loop:none
  /// CHECK-DAG: <<Val2:i\d+>> IntConstant 1               loop:none
  /// CHECK-DAG:               NotEqual [<<ParN>>,<<Val>>] loop:none
  /// CHECK-DAG: <<and:i\d+>>  And [<<ParN>>,<<Val2>>]     loop:none
  /// CHECK-DAG:               Equal [<<and>>,<<Val>>]     loop:none
  /// CHECK-DAG:               Add [<<ParN>>,<<Val1>>]     loop:none
  /// CHECK-DAG: <<Inv1:i\d+>> InvokeVirtual method_name:Main.notInlineableSeries loop:none
  /// CHECK-DAG: <<Add1:i\d+>> Add [<<ParN>>,<<Inv1>>]     loop:none
  /// CHECK-DAG: <<Inv2:i\d+>> InvokeVirtual method_name:Main.notInlineableSeries loop:none
  /// CHECK-DAG: <<Add2:i\d+>> Add [<<Inv2>>,<<Val2>>]     loop:none
  /// CHECK-DAG: <<Add3:i\d+>> Add [<<ParN>>,<<Add2>>]     loop:none
  /// CHECK-DAG: <<Phi1:i\d+>> Phi [<<Add1>>,<<Add3>>]     loop:none
  /// CHECK-DAG: <<Phi2:i\d+>> Phi [<<ParN>>,<<Phi1>>]     loop:none
  /// CHECK-DAG:               Return [<<Phi2>>]           loop:none

  /// CHECK-START: int Main.notInlineableSeries(int) TRE (after)
  /// CHECK-DAG: <<ParN:i\d+>>  ParameterValue              loop:none
  /// CHECK-DAG: <<Val:i\d+>>   IntConstant 0               loop:none
  /// CHECK-DAG: <<Val1:i\d+>>  IntConstant -1              loop:none
  /// CHECK-DAG: <<Val2:i\d+>>  IntConstant 1               loop:none
  /// CHECK-DAG: <<PhiA:i\d+>>  Phi [<<Val>>,<<PhiA1:i\d+>>]  loop:<<Loop:B\d+>>
  /// CHECK-DAG: <<PhiN:i\d+>>  Phi [<<ParN>>,<<PhiN1:i\d+>>] loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:                SuspendCheck                loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:                NotEqual [<<PhiN>>,<<Val>>] loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<and:i\d+>>   And [<<PhiN>>,<<Val2>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG:                Equal [<<and>>,<<Val>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddN1:i\d+>> Add [<<PhiN>>,<<Val1>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddN2:i\d+>> Add [<<PhiN>>,<<Val1>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddA1:i\d+>> Add [<<PhiN>>,<<PhiA>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<Add2:i\d+>>  Add [<<PhiA>>,<<Val2>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddA2:i\d+>> Add [<<PhiN>>,<<Add2>>]     loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<PhiA1>>      Phi [<<AddA1>>,<<AddA2>>]   loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<PhiN1>>      Phi [<<AddN1>>,<<AddN2>>]   loop:<<Loop>> outer_loop:none
  /// CHECK-DAG: <<AddR:i\d+>>  Add [<<PhiA>>,<<PhiN>>]     loop:none
  /// CHECK-DAG:                Return [<<AddR>>]           loop:none
  /// CHECK-NOT:                InvokeVirtual method_name:Main.notInlineableSeries

  public int notInlineableSeries(int paramInt)
  {
    int i = paramInt;
    if (paramInt == 0) {
      return i;
    }
    if ((i & 0x1) != 0) {
      i += notInlineableSeries(paramInt - 1);
    } else {
      i += 1 + notInlineableSeries(paramInt - 1);
    }
    return i;
  }
 
  // This method has two different decrement operations with I/P parameter which are passed to recursive calls.
  // Current TRE implementation don't support it & hence, CFG will be same before & after TRE.
  public int fibonacci(int n)
  {
    if ((n == 0) || (n == 1)) {
      return n;
    } else {
      return fibonacci(n - 1) + (fibonacci(n - 2));
    }
  }
  
  public static void main(String[] args)
  {
    Main obj = new Main();
    assertIntEquals(5050, obj.arithmeticSeries1(100));
    assertIntEquals(5050, obj.arithmeticSeries2(100, 0));
    assertIntEquals(5100, obj.notInlineableSeries(100));
    assertIntEquals(6765, obj.fibonacci(20));
  }
}
