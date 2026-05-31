use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Type_In_Trait_Method_313;

impl Mutator for Replace_Type_In_Trait_Method_313 {
    fn name(&self) -> &str {
        "Replace_Type_In_Trait_Method_313"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = **return_type {
                                let mut rng = thread_rng();
                                let types = vec![
                                    parse_quote!(u8),
                                    parse_quote!(usize),
                                    parse_quote!(i32),
                                    parse_quote!(f64),
                                ];
                                let new_type = types.choose(&mut rng).unwrap();
                                method.sig.output = ReturnType::Type(
                                    Default::default(),
                                    Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: segments.clone(),
                                        },
                                    })),
                                );
                                method.sig.inputs.iter_mut().for_each(|input| {
                                    if let FnArg::Typed(pat_type) = input {
                                        pat_type.ty = Box::new(new_type.clone());
                                    }
                                });
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the type of a trait method with another type. This transformation aims to test the compiler's handling of type checking and const trait implementations."
    }
}