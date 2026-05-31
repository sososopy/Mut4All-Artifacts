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

pub struct Box_And_Yield_Interaction_278;

impl Mutator for Box_And_Yield_Interaction_278 {
    fn name(&self) -> &str {
        "Box_And_Yield_Interaction_278"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                        if let Expr::Block(ref block) = *closure.body {
                            if block.block.stmts.len() == 1 {
                                if let Stmt::Expr(Expr::Yield(_), _) = block.block.stmts[0] {
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
        "The mutation operator identifies closures containing generator expressions using `yield` that are boxed. It modifies these expressions by encapsulating them in a `Box::new` assignment to a mutable variable, enhancing the interaction between boxed generators and mutable references."
    }
}