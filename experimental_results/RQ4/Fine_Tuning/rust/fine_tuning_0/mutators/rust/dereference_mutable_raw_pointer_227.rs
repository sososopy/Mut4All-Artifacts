use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Dereference_Mutable_Raw_Pointer_227;

impl Mutator for Dereference_Mutable_Raw_Pointer_227 {
    fn name(&self) -> &str {
        "Dereference_Mutable_Raw_Pointer_227"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found_mutable_raw_pointer = false;
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            if let Expr::Reference(expr_ref) = &**expr {
                                if expr_ref.mutability.is_some() {
                                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                                        if let Some(ident) = expr_path.path.get_ident() {
                                            if ident == "G" {
                                                found_mutable_raw_pointer = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if found_mutable_raw_pointer {
                    let deref_stmt: Stmt = parse_quote! {
                        let _value = unsafe { *var };
                    };
                    item_fn.block.stmts.insert(1, deref_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions that create a mutable raw pointer to a global variable using `&raw mut`. It then introduces a dereference operation on this pointer within the function, potentially leading to undefined behavior due to unsafety. This transformation is intended to challenge the compiler's handling of raw pointers and unsafe operations."
    }
}