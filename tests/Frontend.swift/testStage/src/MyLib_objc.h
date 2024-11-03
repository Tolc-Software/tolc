#import <Foundation/Foundation.h>

@interface MyLibAnimal : NSObject

- (NSString *)name;
- (void)setName:newName;

- (int)i;
- (void)seti:(int)newi;
- (void)sayHi;

// First one you don't name, the second you do
- (int)add:(int)x y:(int)y;

@property (class, nonatomic, assign, readonly) int userCount;

+ (int)userCount;

@end
