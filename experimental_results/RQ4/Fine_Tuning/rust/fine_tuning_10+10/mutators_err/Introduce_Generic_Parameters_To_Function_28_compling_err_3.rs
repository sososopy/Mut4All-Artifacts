use syn::{Item, Ident, GenericParam, TypeParam, FnArg, PatType, Type, TypePath};
use proc_macro2::Span;
use syn::punctuated::Punctuated;

pub struct Introduce_Generic_Parameters_To_Function_28;

impl Mutator for Introduce_Generic_Parameters_To_Function_28 {
    fn name(&self) -> &str {
        "Introduce_Generic_Parameters_To_Function_28"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(first_param) = func.sig.inputs.first_mut() {
                    let generic_ident = Ident::new("T", Span::call_site());
                    let mut generics = func.sig.generics.clone();
                    generics.params.push(GenericParam::Type(TypeParam {
                        attrs: vec![],
                        ident: generic_ident.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                        eq_token: None,
                        default: None,
                    }));

                    func.sig.generics = generics;
                    
                    if let FnArg::Typed(PatType { ty, .. }) = first_param {
                        *ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: generic_ident.into(),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function definitions and introduces a new generic parameter `<T>` to the function signature. The type of the first parameter is replaced with this generic type `T`. This operator is designed to test the Rust compiler's handling of generics, especially when the function body may not be compatible with the new generic type, potentially leading to type-checking errors."
    }
}