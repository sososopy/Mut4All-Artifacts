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

pub struct Dereference_Mutable_Raw_Pointer_349;

impl Mutator for Dereference_Mutable_Raw_Pointer_349 {
    fn name(&self) -> &str {
        "Dereference_Mutable_Raw_Pointer_349"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found_pointer = false;
                let mut new_stmts = Vec::new();

                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Reference(expr_ref) = &**expr {
                                if expr_ref.mutability.is_some() {
                                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                                        if expr_path.path.segments.last().unwrap().ident == "G" {
                                            found_pointer = true;
                                            new_stmts.push(stmt.clone());
                                            let deref_stmt: Stmt = parse_quote! {
                                                let _value = unsafe { *#expr_path };
                                            };
                                            new_stmts.push(deref_stmt);
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }

                if found_pointer {
                    item_fn.block.stmts = new_stmts;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets a mutable raw pointer created using `&raw mut` syntax. It introduces a dereference operation on this pointer within a function, potentially leading to undefined behavior. The operator identifies a mutable global variable and a function using it, then locates the pointer creation and adds a dereference operation, thereby manipulating or accessing the intended memory location."
    }
}