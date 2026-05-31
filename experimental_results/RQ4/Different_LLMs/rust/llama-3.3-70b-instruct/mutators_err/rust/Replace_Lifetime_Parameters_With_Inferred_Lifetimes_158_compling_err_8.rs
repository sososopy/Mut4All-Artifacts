use syn::parse_quote;
use crate::mutator::Mutator;

struct Replace_Lifetime_Parameters_With_Inferred_Lifetimes_158;

impl Mutator for Replace_Lifetime_Parameters_With_Inferred_Lifetimes_158 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameters_With_Inferred_Lifetimes_158"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if func.sig.generics.params.is_empty() {
                    func.sig.generics = parse_quote! {<>};
                }
                if let Some(generics) = &func.sig.generics {
                    let params: Vec<syn::GenericParam> = generics
                        .params
                        .into_iter()
                        .filter_map(|param| match param {
                            syn::GenericParam::Lifetime(_) => None,
                            _ => Some(param),
                        })
                        .collect();
                    func.sig.generics = parse_quote! {<#(#params),*>};
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.generics.params.is_empty() {
                            func.sig.generics = parse_quote! {<>};
                        }
                        if let Some(generics) = &func.sig.generics {
                            let params: Vec<syn::GenericParam> = generics
                                .params
                                .into_iter()
                                .filter_map(|param| match param {
                                    syn::GenericParam::Lifetime(_) => None,
                                    _ => Some(param),
                                })
                                .collect();
                            func.sig.generics = parse_quote! {<#(#params),*>};
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with explicit lifetime parameters and replaces them with inferred lifetimes. This transformation tests the compiler's ability to correctly infer lifetimes and handle related constraints, potentially exposing bugs related to lifetime parameters and inference."
    }
}