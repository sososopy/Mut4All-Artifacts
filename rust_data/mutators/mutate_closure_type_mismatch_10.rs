use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Mutate_Closure_Type_Mismatch_10;

impl Mutator for Mutate_Closure_Type_Mismatch_10 {
    fn name(&self) -> &str {
        "Mutate_Closure_Type_Mismatch_10"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if segment.ident == "FnMut" {
                                    if let Some(Expr::Closure(closure)) = item_fn.block.stmts.iter_mut().find_map(|stmt| {
                                        if let Stmt::Expr(expr, _) = stmt {
                                            Some(expr)
                                        } else {
                                            None
                                        }
                                    }) {
                                        if let Expr::Block(block) = &mut *closure.body {
                                            block.block.stmts.push(parse_quote! {
                                                "error"
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

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets closures within functions that accept closures as arguments. It modifies the closure to introduce a type mismatch by adding a string return value, causing a discrepancy between the expected and actual behavior of the closure."
    }
}