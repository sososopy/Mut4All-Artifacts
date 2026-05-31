use syn::{parse_quote, Item, ReturnType, Type, TypePath, GenericParam, punctuated::Punctuated};
use crate::mutator::Mutator;

struct Replace_Const_Generic_With_Non_Generic_343;

impl Mutator for Replace_Const_Generic_With_Non_Generic_343 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Generic_343"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if !trait_item.generics.params.is_empty() {
                    let mut new_generics = trait_item.generics.clone();
                    new_generics.params = Punctuated::new();
                    let mut new_trait_item = trait_item.clone();
                    new_trait_item.generics = new_generics;
                    for item in &mut trait_item.items {
                        match item {
                            syn::TraitItem::Method(method) => {
                                let mut new_method = method.clone();
                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                    if let Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments,
                                        },
                                    }) = return_type.as_ref() {
                                        if let Some(segment) = segments.iter().next() {
                                            for param in &trait_item.generics.params {
                                                if let GenericParam::Const(param) = param {
                                                    if param.ident == segment.ident {
                                                        if let Some(replacement) = param.default {
                                                            let new_return_type = parse_quote! { #replacement };
                                                            new_method.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                new_trait_item.items.push(item.clone());
                            },
                            _ => {
                                new_trait_item.items.push(item.clone());
                            }
                        }
                    }
                    *item = Item::Trait(new_trait_item);
                }
            }
            if let Item::Impl(impl_item) = item {
                if !impl_item.generics.params.is_empty() {
                    let mut new_generics = impl_item.generics.clone();
                    new_generics.params = Punctuated::new();
                    let mut new_impl_item = impl_item.clone();
                    new_impl_item.generics = new_generics;
                    for impl_item in &mut impl_item.items {
                        match impl_item {
                            syn::ImplItem::Method(method) => {
                                let mut new_method = method.clone();
                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                    if let Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments,
                                        },
                                    }) = return_type.as_ref() {
                                        if let Some(segment) = segments.iter().next() {
                                            for param in &impl_item.generics.params {
                                                if let GenericParam::Const(param) = param {
                                                    if param.ident == segment.ident {
                                                        if let Some(replacement) = param.default {
                                                            let new_return_type = parse_quote! { #replacement };
                                                            new_method.sig.output = ReturnType::Type(Default::default(), Box::new(new_return_type));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                new_impl_item.items.push(impl_item.clone());
                            },
                            _ => {
                                new_impl_item.items.push(impl_item.clone());
                            }
                        }
                    }
                    *item = Item::Impl(new_impl_item);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-generic const values in trait and impl definitions. It aims to test the compiler's handling of const generics and their interactions with trait and impl definitions."
    }
}