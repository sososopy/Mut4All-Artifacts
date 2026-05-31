use syn::parse_quote;
use crate::mutator::Mutator;

pub struct AddAsyncConstRefGeneric_103;

impl Mutator for AddAsyncConstRefGeneric_103 {
    fn name(&self) -> &str {
        "AddAsyncConstRefGeneric_103"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    let const_param = parse_quote!(const T: &'static str);
                    func.sig.generics.params.push(const_param);
                    func.sig.asyncness = Some(syn::token::Async::default());
                    func.sig.output = syn::ReturnType::Type(
                        syn::token::RArrow::default(),
                        Box::new(parse_quote!(impl Future<Output = ()>)),
                    );
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms non-async functions by adding a const generic reference parameter, marking the function as async, and changing the return type to impl Future<Output = ()>. This forces the compiler to handle async function signatures with const generics, testing the integration of async/await with const parameters and opaque return types."
    }
}