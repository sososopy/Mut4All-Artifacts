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

pub struct Modify_Static_Init_With_Drop_128;

impl Mutator for Modify_Static_Init_With_Drop_128 {
    fn name(&self) -> &str {
        "Modify_Static_Init_With_Drop_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Expr::Block(expr_block) = &mut *item_static.expr {
                    let mut new_stmts = vec![];
                    for stmt in &expr_block.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(LocalInit { expr, .. }) = &local.init {
                                if let Expr::Array(_) = &**expr {
                                    // Transform the initialization
                                    let new_stmt: Stmt = parse_quote! {
                                        let mut test_vec: Vec<String> = Vec::new();
                                        test_vec.push(String::from("Example"));
                                    };
                                    new_stmts.push(new_stmt);
                                    continue;
                                }
                            }
                        }
                        new_stmts.push(stmt.clone());
                    }
                    expr_block.block.stmts = new_stmts;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets static variable initialization blocks that involve data structures with Drop implementations. By introducing a vector and directly pushing elements into it without prior initialization, it stresses the compiler's handling of uninitialized memory for types with destructors. This can reveal potential issues in the compiler's management of partially initialized structures in static contexts."
    }
}