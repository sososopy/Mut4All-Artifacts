use std::collections::HashMap;
use syn::parse_quote;
use syn::Item;
use syn::Type;
use syn::punctuated::Punctuated;
use syn::FnArg;
use syn::PatType;
use syn::TypePath;
use syn::GenericParam;
use syn::Generics;
use crate::mutator::Mutator;

pub struct ModifyOpaqueTypeUsage113;

impl Mutator for ModifyOpaqueTypeUsage113 {
    fn name(&self) -> &str {
        "ModifyOpaqueTypeUsage113"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = HashMap::new();

        for item in &file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(impl_trait) = &*type_item.ty {
                    type_aliases.insert(type_item.ident.clone(), impl_trait.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_inputs = Punctuated::new();
                let mut new_generics = func.sig.generics.clone();

                for input in &func.sig.inputs {
                    if let FnArg::Typed(PatType { pat, ty, .. }) = input {
                        if let Type::Path(TypePath { path, .. }) = &**ty {
                            if let Some(impl_trait) = type_aliases.get(&path.segments.last().unwrap().ident) {
                                let generic_param: GenericParam = parse_quote! { T: #impl_trait };
                                new_generics.params.push(generic_param);

                                let new_arg: FnArg = parse_quote! { #pat: T };
                                new_inputs.push(new_arg);
                                continue;
                            }
                        }
                    }
                    new_inputs.push(input.clone());
                }

                func.sig.inputs = new_inputs;
                func.sig.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator identifies type aliases using opaque types (impl Trait) and modifies function signatures to replace these aliases with explicit generic parameters that directly specify the trait bounds. This ensures that functions explicitly handle trait constraints, potentially uncovering issues with trait handling in the Rust compiler."
    }
}