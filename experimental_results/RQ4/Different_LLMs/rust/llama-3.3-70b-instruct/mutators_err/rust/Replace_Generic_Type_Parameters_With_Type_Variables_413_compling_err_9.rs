use syn::{parse_quote, Generics, GenericParam, Lifetime, LifetimeParam, Token, punctuated::Punctuated};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Generic_Type_Parameters_With_Type_Variables_413;

impl Mutator for Replace_Generic_Type_Parameters_With_Type_Variables_413 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameters_With_Type_Variables_413"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                match &mut struct_item.generics {
                    syn::Generics::WhereClause(where_clause) => {
                        for generic_param in &mut where_clause.predicates {
                            if let syn::GenericParam::Type(type_param) = generic_param {
                                let lifetime = Lifetime::new(&format!("'{}", type_param.ident), Span::call_site());
                                let lifetime_param = LifetimeParam {
                                    attrs: vec![],
                                    lifetime: lifetime,
                                    bounds: Punctuated::new(),
                                    colon_token: Some(Token![:](Span::call_site())),
                                };
                                struct_item.generics.params.push(GenericParam::Lifetime(lifetime_param));
                            }
                        }
                    }
                    syn::Generics::None => {
                        let mut params = Punctuated::new();
                        let lifetime = Lifetime::new("'a", Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: lifetime,
                            bounds: Punctuated::new(),
                            colon_token: Some(Token![:](Span::call_site())),
                        };
                        params.push(GenericParam::Lifetime(lifetime_param));
                        struct_item.generics = syn::Generics {
                            params: params,
                            where_clause: None,
                            gt_token: Some(Token![>](Span::call_site())),
                            lt_token: Some(Token![<](Span::call_site())),
                        };
                    }
                    syn::Generics::Angles(angles) => {
                        for generic_param in &mut angles.params {
                            if let GenericParam::Type(type_param) = generic_param {
                                let lifetime = Lifetime::new(&format!("'{}", type_param.ident), Span::call_site());
                                let lifetime_param = LifetimeParam {
                                    attrs: vec![],
                                    lifetime: lifetime,
                                    bounds: Punctuated::new(),
                                    colon_token: Some(Token![:](Span::call_site())),
                                };
                                *generic_param = GenericParam::Lifetime(lifetime_param);
                            }
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                for generic_param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = generic_param {
                        let lifetime = Lifetime::new(&format!("'{}", type_param.ident), Span::call_site());
                        let lifetime_param = LifetimeParam {
                            attrs: vec![],
                            lifetime: lifetime,
                            bounds: Punctuated::new(),
                            colon_token: Some(Token![:](Span::call_site())),
                        };
                        *generic_param = GenericParam::Lifetime(lifetime_param);
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for generic_param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = generic_param {
                                let lifetime = Lifetime::new(&format!("'{}", type_param.ident), Span::call_site());
                                let lifetime_param = LifetimeParam {
                                    attrs: vec![],
                                    lifetime: lifetime,
                                    bounds: Punctuated::new(),
                                    colon_token: Some(Token![:](Span::call_site())),
                                };
                                *generic_param = GenericParam::Lifetime(lifetime_param);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic type parameters with type variables in type definitions, function signatures, and trait bounds. It aims to create complex type relationships that may expose bugs in the compiler's type inference and trait resolution systems."
    }
}