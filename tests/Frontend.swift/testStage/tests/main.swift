import MyLib_swift


var animal: MyLib.Animal = MyLib.Animal()
// print(animal.sayHi())
assert(animal.name != nil)
assert(animal.name == "Ior")

print("The name of the animal is: \(animal.name as Optional)")
// animal.setName("Pooh")
// print("The new name of the animal is: \(animal.name() as Optional)")
// print("UserCount is a static variable: \(MyLibAnimal.userCount)")
// MyLib_Animal.userCount = 5
// print("UserCount is a static variable: \(MyLibAnimal.userCount)")
// print("Hello from Swift")

