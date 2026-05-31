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

pub struct Change_Type_Of_Trait_Method_389;

impl Mutator for Change_Type_Of_Trait_Method_389 {
    fn name(&self) -> &str {
        "Change_Type_Of_Trait_Method_389"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, _) = func.sig.output {
                            for input in &mut func.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    let new_type = match *pat_type.ty {
                                        Type::Path(TypePath {
                                            qself: None,
                                            ref path,
                                        }) => {
                                            let segment = path.segments.last().unwrap();
                                            let new_ident = match segment.ident.to_string().as_str() {
                                                "u8" => "usize",
                                                "usize" => "u8",
                                                "i32" => "i64",
                                                "i64" => "i32",
                                                _ => &segment.ident.to_string(),
                                            };
                                            Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: punctuated::Punctuated::from_iter(vec![
                                                        PathSegment {
                                                            ident: Ident::new(
                                                                new_ident,
                                                                Span::call_site(),
                                                            ),
                                                            arguments: PathArguments::None,
                                                        },
                                                    ]),
                                                },
                                            })
                                        }
                                        _ => *pat_type.ty.clone(),
                                    };
                                    pat_type.ty = Box::new(new_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method definitions and changes the type of one of its parameters to a different type. It applies to code constructs that involve trait definitions and method implementations. This transformation can potentially expose bugs related to type errors or incorrect handling of different data types."
    }
}