public class MyLib {
    public class Animal {
        private var animal: MyLibAnimal

        public init() {
            animal = MyLibAnimal()
        }

        public function roar(sound: String) -> String {
            return sound
        }

        public var name: String? {
            get {
                return animal.name()
            }
            set(newName) {
                animal.setName(newName)
            }
        }

        public var i: Int32 {
            get {
                return animal.i()
            }
            set(newi) {
                animal.seti(newi)
            }
        }
    }
}

// var animal: MyLibAnimal = MyLibAnimal()
// print(animal.sayHi())
// print("The name of the animal is: \(animal.name() as Optional)")
// animal.setName("Pooh")
// print("The new name of the animal is: \(animal.name() as Optional)")
// print("UserCount is a static variable: \(MyLibAnimal.userCount)")
// MyLib_Animal.userCount = 5
// print("UserCount is a static variable: \(MyLibAnimal.userCount)")
// print("Hello from ObjcSwift")

