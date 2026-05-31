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

pub struct Box_Yield_Interaction_121;

impl Mutator for Box_Yield_Interaction_121 {
    fn name(&self) -> &str {
        "Box_Yield_Interaction_121"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                        if closure.inputs.is_empty() {
                            if let Expr::Box(box_expr) = &*closure.body {
                                if let Expr::Yield(_) = &*box_expr.expr {
                                    let new_stmt: Stmt = parse_quote! {
                                        let mut gen_box = Box::new(|| yield);
                                    };
                                    new_stmts.push(new_stmt);
                                    continue;
                                }
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures containing a boxed generator expression (using `yield`). It replaces such expressions with a mutable variable assignment using `Box::new`, encapsulating the generator. This transformation tests the interaction between boxed generators and mutable references, potentially revealing issues with Rust's borrowing and ownership rules."
    }
}