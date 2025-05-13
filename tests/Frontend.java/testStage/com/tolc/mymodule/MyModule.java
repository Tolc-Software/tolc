package com.tolc.mymodule;

import java.lang.ref.Cleaner;

public class MyModule {
    // Used to clean up native allocations
    private final static Cleaner tolcCleaner = Cleaner.create();

    // New and delete operators
    private static native long tolcNewMyModuleCar();
    private static native void tolcDeleteMyModuleCar(long pointer);

    static {
        // NOTE: This has to be found with e.g.
        //       -Djava.library.path=./path/to/lib/dir
        //       In that directory there has to be e.g. libmyModule.dylib for MacOS
        System.loadLibrary("myModule");
    }

    // Class will be passed to C++ from Java
    public static class DataFromJava {
        public int a = 1;
        public int b = 2;
    }
    // Note the 'native' tag
    public native void passJavaObjectToCPP(DataFromJava data);

    public native String passStringToCPP(String s);
    public native String returnStringFromCPP();

    // Inner class
    public class Car {
        // Removes the instance
        protected final Cleaner.Cleanable cleanable;

        public Car()
        {
            System.out.println("Created a new car in Java");
            long cppCar = tolcNewMyModuleCar();
            cleanable = tolcCleaner.register(this, () -> {
                System.out.println("Deleting the car from the cleaner interface");
                tolcDeleteMyModuleCar(cppCar);
            });
        }
    }

    public static void main(String[] args) {
        System.out.println("Hello! You should run the test class instead via the shell script");
    }

}
