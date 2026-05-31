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

pub struct Replace_Return_With_Diverging_451;

impl Mutator for Replace_Return_With_Diverging_451 {
    fn name(&self) -> &str {
        "Replace_Return_With_Diverging_451"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if let Some(last_segment) = type_path.path.segments.last() {
                            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::Type(ty) = arg {
                                        if let Type::Path(ty_path) = &**ty {
                                            if ty_path.path.segments.len() == 2 {
                                                let first_segment = &ty_path.path.segments[0];
                                                let first_ident = &first_segment.ident;
                                                let has_type_param = item_fn.sig.generics.params.iter().any(|param| {
                                                    if let GenericParam::Type(type_param) = param {
                                                        type_param.ident == *first_ident
                                                    } else {
                                                        false
                                                    }
                                                });
                                                if has_type_param {
                                                    let has_trait_bound = item_fn.sig.generics.params.iter().any(|param| {
                                                        if let GenericParam::Type(type_param) = param {
                                                            type_param.ident == *first_ident && !type_param.bounds.is_empty()
                                                        } else {
                                                            false
                                                        }
                                                    });
                                                    if has_trait_bound {
                                                        let diverging_expr = parse_quote! { loop {} };
                                                        let mut new_block = item_fn.block.clone();
                                                        new_block.stmts.clear();
                                                        new_block.stmts.push(Stmt::Expr(diverging_expr, None));
                                                        item_fn.block = new_block;
                                                        break;
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