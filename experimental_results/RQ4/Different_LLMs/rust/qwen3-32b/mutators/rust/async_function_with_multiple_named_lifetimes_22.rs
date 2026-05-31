use crate::mutator::Mutator;
use syn::Lifetime;
use syn::FnArg;
use syn::Type;
use std::collections::HashSet;
use syn::GenericParam;
use syn::LifetimeParam;
use syn::punctuated::Punctuated;

pub struct Async_Function_With_Multiple_Named_Lifetimes_22;

impl Mutator for Async_Function_With_Multiple_Named_Lifetimes_22 {
    fn name(&self) -> &str {
        "Async_Function_With_Multiple_Named_Lifetimes_22"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_none() {
                    let mut lifetimes: Vec<&Lifetime> = vec![];
                    for input in &func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Reference(ref_type) = &*pat_type.ty {
                                if let Some(lt) = &ref_type.lifetime {
                                    lifetimes.push(lt);
                                }
                            }
                        }
                    }
                    let unique_lifetimes: HashSet<_> = lifetimes.iter().collect();
                    if unique_lifetimes.len() >= 2 {
                        func.sig.asyncness = Some(syn::token::Async {
                            span: proc_macro2::Span::call_site(),
                        });
                        for lt in &unique_lifetimes {
                            let lt_ident = &lt.ident;
                            let mut found = false;
                            for param in &func.sig.generics.params {
                                if let GenericParam::Lifetime(lt_param) = param {
                                    if lt_param.lifetime.ident == *lt_ident {
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if !found {
                                let new_lt = LifetimeParam {
                                    attrs: vec![],
                                    lifetime: Lifetime {
                                        apostrophe: proc_macro2::Span::call_site().into(),
                                        ident: lt.ident.clone(),
                                    },
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                };
                                func.sig.generics.params.push(GenericParam::Lifetime(new_lt));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}