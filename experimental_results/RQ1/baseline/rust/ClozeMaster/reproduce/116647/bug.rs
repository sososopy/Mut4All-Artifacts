trait TypeVal<T> {
    const VAL: T;
}
struct Five;
struct Multiply<N, M> {
    _n: PhantomData,
}
impl<N, M> TypeVal<usize> for Multiply<N, M> where N: TypeVal<VAL> {}
fn main() {
    let mut x: Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multiply<Five, Multi}