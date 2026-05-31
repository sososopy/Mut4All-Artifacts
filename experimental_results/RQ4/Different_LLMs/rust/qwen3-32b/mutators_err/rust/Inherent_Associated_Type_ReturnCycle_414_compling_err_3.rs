use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Inherent_Associated_Type_ReturnCycle_414;

impl Mutator for Inherent_Associated_Type_ReturnCycle_414 {
    fn name(&self) -> &str {
        "Inherent_Associated_Type_ReturnCycle_414"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                let has_type_params = impl_item.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Type(_))
                });
                if !has_type_params {
                    continue;
                }

                let type_params: HashSet<String> = impl_item.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.to_string())
                        } else {
                            None
                        }
                    })
                    .collect();

                let mut added_associated_type = false;

                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let syn::Type::Path(type_path) = &**return_type {
                                if type_path.path.segments.len() == 1 {
                                    let segment = &type_path.path.segments[0];
                                    let ident_str = segment.ident.to_string();
                                    if type_params.contains(&ident_str) {
                                        let new_path = syn::Path {
                                            leading_colon: None,
                                            segments: {
                                                let mut segs = Punctuated::new();
                                                segs.push(syn::PathSegment {
                                                    ident: Ident::new("Self", Span::call_site()),
                                                    arguments: syn::PathArguments::None,
                                                });
                                                segs.push(syn::PathSegment {
                                                    ident: Ident::new("Item", Span::call_site()),
                                                    arguments: syn::PathArguments::None,
                                                });
                                                segs
                                            },
                                        };
                                        *return_type = Box::new(syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: new_path,
                                        }));

                                        if !added_associated_type {
                                            let default_type = syn::Type::Path(syn::TypePath {
                                                qself: None,
                                                path: syn::Path {
                                                    leading_colon: None,
                                                    segments: {
                                                        let mut segs = Punctuated::new();
                                                        segs.push(syn::PathSegment {
                                                            ident: Ident::new(&ident_str, Span::call_site()),
                                                            arguments: syn::PathArguments::None,
                                                        });
                                                        segs
                                                    },
                                                },
                                            });

                                            let item_type = parse_quote! {
                                                type Item: Default = #default_type;
                                            };

                                            impl_item.items.push(syn::ImplItem::Type(item_type));

                                            added_associated_type = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms methods in generic impl blocks that return type parameters into methods returning an inherent associated type aliasing the type parameter. This creates a cycle in function signature resolution and increases the likelihood of cycle detection by adding a trait bound to the associated type. It stresses the compiler's handling of associated types and recursive type resolution."
    }
}