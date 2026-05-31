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

pub struct Swap_Type_Params_In_Transmute_Call_45;

impl Mutator for Swap_Type_Params_In_Transmute_Call_45 {
    fn name(&self) -> &str {
        "Swap_Type_Params_In_Transmute_Call_45"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                            if path.segments.last().map_or(false, |segment| segment.ident == "is_maybe_transmutable") {
                                if let Some(segment) = path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(angle_bracketed) = &mut segment.arguments {
                                        if angle_bracketed.args.len() == 2 {
                                            let args: Vec<_> = angle_bracketed.args.iter().cloned().collect();
                                            if let [GenericArgument::Type(_), GenericArgument::Type(_)] = &args[..] {
                                                let mut new_args = Punctuated::new();
                                                new_args.push(args[1].clone());
                                                new_args.push(args[0].clone());
                                                segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                    colon2_token: angle_bracketed.colon2_token,
                                                    lt_token: angle_bracketed.lt_token,
                                                    args: new_args,
                                                    gt_token: angle_bracketed.gt_token,
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
    
    fn chain_of_thought(&self) -> &str {
        ""
    }
}