#import "myLib.h"

int main() {
	@autoreleasepool {
		NSString* greeting = [myLibmyLib getGreeting];
		assert([greeting isEqualToString:@"Hi Tolc!"]);
	}
}
