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

pub struct Modify_Slice_Indexing_In_Functions_356;

impl Mutator for Modify_Slice_Indexing_In_Functions_356 {
    fn name(&self) -> &str {
        "Modify_Slice_Indexing_In_Functions_356"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = SliceIndexVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function bodies that access slices. It modifies slice indexing to use an out-of-bounds index, specifically one more than the maximum valid index. This aims to introduce runtime panics due to out-of-bounds access, testing the robustness of slice handling and error reporting in Rust."
    }
}

struct SliceIndexVisitor;

impl VisitMut for SliceIndexVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Index(expr_index) = expr {
            if let Expr::Path(ref path) = *expr_index.expr {
                if let Some(segment) = path.path.segments.last() {
                    if segment.ident == "buffer" {
                        let new_index: Expr = parse_quote!(self.buffer.len() + 1);
                        expr_index.index = Box::new(new_index);
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}