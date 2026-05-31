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

pub struct Modify_Slice_Indexing_Out_Of_Bounds_96;

impl Mutator for Modify_Slice_Indexing_Out_Of_Bounds_96 {
    fn name(&self) -> &str {
        "Modify_Slice_Indexing_Out_Of_Bounds_96"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = SliceIndexingVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets slice or vector indexing within function bodies. It modifies the indexing to access an out-of-bounds range, provoking potential runtime errors or panics. This transformation tests the compiler's and runtime's handling of boundary checks and error reporting."
    }
}

struct SliceIndexingVisitor;

impl VisitMut for SliceIndexingVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Index(expr_index) = expr {
            if let Expr::Path(ref path) = *expr_index.expr {
                if path.path.segments.last().unwrap().ident == "buffer" {
                    let new_index: Expr = parse_quote! { self.buffer.len()..self.buffer.len() + 1 };
                    expr_index.index = Box::new(new_index);
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}