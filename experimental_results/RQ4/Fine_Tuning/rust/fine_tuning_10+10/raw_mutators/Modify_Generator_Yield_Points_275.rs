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

pub struct Modify_Generator_Yield_Points_275;

impl Mutator for Modify_Generator_Yield_Points_275 {
    fn name(&self) -> &str {
        "Modify_Generator_Yield_Points_275"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(block) = &mut item_fn.block {
                    let mut generator_visitor = GeneratorVisitor::new();
                    generator_visitor.visit_block_mut(block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets generator functions, specifically the yield points within the generator construct. It modifies the yield expression to involve a more complex or varying set of operations, potentially triggering compiler crashes related to the handling of generator state. The mutation involves using non-trivially sized types or additional operations in the yield statement."
    }
}

struct GeneratorVisitor;

impl GeneratorVisitor {
    fn new() -> Self {
        GeneratorVisitor
    }
}

impl VisitMut for GeneratorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Yield(expr_yield) = expr {
            if expr_yield.expr.is_none() {
                let non_sized_expr: Expr = parse_quote! {
                    {
                        let non_sized = *"";
                        non_sized.len()
                    }
                };
                expr_yield.expr = Some(Box::new(non_sized_expr));
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}