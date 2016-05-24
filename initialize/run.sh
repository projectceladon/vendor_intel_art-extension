function build_infrastructure() {
  echo "Ensure we have our automator script ready"
  make
  adb install -r bin/initialization-debug-free-unsigned.apk
}

adb root
build_infrastructure
adb shell am instrument -w com.intel.initialization/android.test.InstrumentationTestRunner
