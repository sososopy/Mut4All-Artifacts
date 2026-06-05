#![feature(const_trait_impl)]

#![allow(incomplete_features)]

pub trait Foo {
    async fn foo(&mut self);
}

impl<T: Foo> Foo  for &mut T{
    async fn foo(&mut self) {
        match () {
            const { (|| {()})() } => {}
        }
    }
}