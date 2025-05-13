package com.tolc.mymodule;

import static org.junit.jupiter.api.Assertions.assertEquals;

import com.tolc.mymodule.MyModule;

import org.junit.jupiter.api.Test;

public class MyModuleTest {

    private final MyModule myModule = new MyModule();

    @Test
    void passStructFromJavaToCPP() {
        MyModule.DataFromJava data = new MyModule.DataFromJava();
        // Initial values
        assertEquals(data.a, 1);
        assertEquals(data.b, 2);
        myModule.passJavaObjectToCPP(data);
        // After passing
        assertEquals(data.a, 100);
        assertEquals(data.b, 200);
    }

    // public native void passStringToCPP(String s);
    // public native String returnStringFromCPP();
    @Test
    void passStrings() {
        String fromCPP = myModule.returnStringFromCPP();
        assertEquals(fromCPP, "Hello from C++!");

        String passedFromCPP = myModule.passStringToCPP("Hello");
        assertEquals(passedFromCPP, "Hello World!");
    }


    // Car
    @Test
    void buildingCars() {
        MyModule.Car car = myModule.new Car();
        car = null;
        System.gc();
    }
}

