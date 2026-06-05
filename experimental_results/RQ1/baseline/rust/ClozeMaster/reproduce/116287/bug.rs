trait MyTrait {
    type MyType;
    fn my_function(&self) -> Self::MyType {
        
    }
}

impl MyTrait for <i32 as MyTrait>::MyType {}


trait AnotherTrait {
    fn another_function(&self) {}
}

impl AnotherTrait for <() as MyTrait>::MyType {}
impl AnotherTrait for <i32 as MyTrait>::MyType {}

fn main() {
    ().my_function().another_function();
}