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

pub struct Modify_Trait_Bounds_450;

impl Mutator for Modify_Trait_Bounds_450 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_450"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, ref mut path, _)) = item_impl.trait_ {
                    for segment in &mut path.segments {
                        if let syn::PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                            for arg in &mut args.args {
                                if let syn::GenericArgument::Type(syn::Type::Path(type_path)) = arg {
                                    for param in &mut type_path.path.segments {
                                        if let syn::PathArguments::AngleBracketed(ref mut args) = param.arguments {
                                            for arg in &mut args.args {
                                                if let syn::GenericArgument::Type(syn::Type::Path(type_path)) = arg {
                                                    for param in &mut type_path.path.segments {
                                                        if let syn::PathArguments::None = param.arguments {
                                                            param.arguments = syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                                colon2_token: None,
                                                                lt_token: token::Lt::default(),
                                                                args: {
                                                                    let mut args = Punctuated::new();
                                                                    args.push(syn::GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                                                        qself: None,
                                                                        path: syn::Path {
                                                                            leading_colon: None,
                                                                            segments: {
                                                                                let mut segs = Punctuated::new();
                                                                                segs.push(syn::PathSegment {
                                                                                    ident: Ident::new("Sync", Span::call_site()),
                                                                                    arguments: syn::PathArguments::None,
                                                                                });
                                                                                segs
                                                                            },
                                                                        },
                                                                    })));
                                                                    args
                                                                },
                                                                gt_token: token::Gt::default(),
                                                            });
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}