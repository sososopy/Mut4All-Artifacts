use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Assoc_Type_With_Nested_Binder_413;

impl Mutator for Replace_Assoc_Type_With_Nested_Binder_413 {
    fn name(&self) -> &str {
        "Replace_Assoc_Type_With_Nested_Binder_413"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = Box::as_mut(&mut pat_type.ty) {
                            if let Some((seg, arg_path)) = type_path.path.segments.iter_mut().find_map(|seg| {
                                if seg.ident == "Fn" {
                                    if let PathArguments::Parenthesized(parens) = &mut seg.arguments {
                                        for arg in &mut parens.inputs {
                                            if let Type::Path(arg_path) = arg {
                                                if arg_path.path.segments.len() == 2 {
                                                    return Some((seg.clone(), arg_path));
                                                }
                                            }
                                        }
                                    }
                                }
                                None
                            }) {
                                let first_segment = seg;
                                let second_segment = arg_path.path.segments[1].clone();
                                let qself = {
                                    let qself_type = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![first_segment.clone()]),
                                        },
                                    });
                                    QSelf {
                                        ty: Box::new(qself_type),
                                        as_token: None,
                                        lt_token: token::Lt::default(),
                                        gt_token: token::Gt::default(),
                                        position: 0,
                                    }
                                };
                                let new_path = {
                                    let mut segments = Punctuated::new();
                                    let assoc_segment = PathSegment {
                                        ident: second_segment.ident.clone(),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::new(),
                                            gt_token: token::Gt::default(),
                                        }),
                                    };
                                    segments.push(assoc_segment);
                                    let inner_segment = PathSegment {
                                        ident: Ident::new("Inner", Span::call_site()),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![GenericArgument::Lifetime(Lifetime::new(
                                                &"<'_>",
                                                Span::call_site(),
                                            ))]),
                                            gt_token: token::Gt::default(),
                                        }),
                                    };
                                    segments.push(inner_segment);
                                    SynPath {
                                        leading_colon: None,
                                        segments,
                                    }
                                };
                                *arg_path = TypePath {
                                    qself: Some(qself),
                                    path: new_path,
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets higher-ranked function parameters (e.g., for<'a> fn(...)) and replaces inherent associated types (T::Assoc) with a nested region reference under a binder that directly captures the bound lifetime. This transformation forces the compiler to resolve an associated type nested under a binder with a directly captured lifetime, increasing the likelihood of region conversion errors and ICEs by creating complex, invalid type relationships that stress the trait and lifetime resolution systems."
    }
}