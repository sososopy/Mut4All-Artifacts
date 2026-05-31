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

pub struct Yield_With_Empty_Block_Tuple_123;

impl Mutator for Yield_With_Empty_Block_Tuple_123 {
    fn name(&self) -> &str {
        "Yield_With_Empty_Block_Tuple_123"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = YieldMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets yield expressions inside closures. It transforms 'yield expr' to 'yield ( {} , expr )' and 'yield' to 'yield ( {} )', introducing an empty block as a tuple element. This creates a malformed generator state by placing a statement-like block inside a tuple, aiming to trigger internal compiler errors in generator interior analysis due to unexpected tuple structure."
    }
}

struct YieldMutatorVisitor;

impl VisitMut for YieldMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Yield(yield_expr) = expr {
            let original_expr = yield_expr.expr.clone();
            let tuple_elements = Punctuated::new();
            let mut tuple_elements = tuple_elements;
            tuple_elements.push(Expr::Block(ExprBlock {
                attrs: Vec::new(),
                label: None,
                block: Block {
                    brace_token: token::Brace::default(),
                    stmts: Vec::new(),
                },
            }));
            if let Some(orig_expr) = original_expr {
                tuple_elements.push(*orig_expr);
            }
            let tuple_expr = Expr::Tuple(ExprTuple {
                attrs: Vec::new(),
                paren_token: token::Paren::default(),
                elems: tuple_elements,
            });
            yield_expr.expr = Some(Box::new(tuple_expr));
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}