#import <Foundation/Foundation.h>
#import "MyLib_objc.h"

int main() {
  @autoreleasepool {
    MyLibAnimal* ior = [[MyLibAnimal alloc] init];
    [ior sayHi];
  }
  return 0;
}
