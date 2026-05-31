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

pub struct Modify_Generator_Yield_118;

impl Mutator for Modify_Generator_Yield_118 {
    fn name(&self) -> &str {
        "Modify_Generator_Yield_118"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let block = &mut item_fn.block;
                for stmt in &mut block.stmts {
                    if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                        if let Expr::Block(expr_block) = &mut *closure.body {
                            let mut new_stmts = Vec::new();
                            for inner_stmt in expr_block.block.stmts.iter() {
                                if let Stmt::Expr(Expr::Yield(expr_yield), _) = inner_stmt {
                                    let non_sized_expr: Expr = parse_quote! {
                                        {
                                            let non_sized = *"";
                                            non_sized.len()
                                        }
                                    };
                                    let mut new_yield = expr_yield.clone();
                                    new_yield.expr = Some(Box::new(non_sized_expr));
                                    new_stmts.push(Stmt::Expr(Expr::Yield(new_yield), None));
                                } else {
                                    new_stmts.push(inner_stmt.clone());
                                }
                            }
                            expr_block.block.stmts = new_stmts;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets generator functions and modifies the yield points by replacing simple yield expressions with more complex operations involving non-sized types. This transformation is intended to explore potential weaknesses in the compiler's handling of generator state and non-trivial type operations, potentially revealing issues with type layout or state management."
    }
}