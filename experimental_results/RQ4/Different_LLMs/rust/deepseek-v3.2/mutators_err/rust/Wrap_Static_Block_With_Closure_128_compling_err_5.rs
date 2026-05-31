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

pub struct Wrap_Static_Block_With_Closure_128;

impl Mutator for Wrap_Static_Block_With_Closure_128 {
    fn name(&self) -> &str {
        "Wrap_Static_Block_With_Closure_128"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Expr::Block(block_expr) = &*item_static.expr {
                    let closure_expr: Expr = parse_quote! { (|| {})() };
                    let mut closure_block = match closure_expr {
                        Expr::Call(call_expr) => {
                            match *call_expr.func {
                                Expr::Closure(closure) => closure.body,
                                _ => panic!("Expected closure"),
                            }
                        }
                        _ => panic!("Expected call expression"),
                    };
                    closure_block = Box::new(Expr::Block(block_expr.block.clone()));
                    let new_expr: Expr = parse_quote! { (|| { #closure_block })() };
                    *item_static.expr = new_expr;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static items with block expression initializers. It wraps the block in an immediately-invoked closure that returns unit, altering drop scope boundaries. This transformation modifies drop elaboration in static contexts, potentially exposing bugs in the compiler's handling of destructors and drop glue within static initializers."
    }
}