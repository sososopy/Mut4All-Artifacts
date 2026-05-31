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

pub struct Modify_Slice_Indexing_157;

impl Mutator for Modify_Slice_Indexing_157 {
    fn name(&self) -> &str {
        "Modify_Slice_Indexing_157"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct SliceIndexMutator;
        
        impl VisitMut for SliceIndexMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(expr_index) = expr {
                    if let Expr::Path(ref path) = *expr_index.expr {
                        if path.path.segments.last().map_or(false, |segment| segment.ident == "len") {
                            let new_expr = parse_quote! { #expr_index.expr.len() + 1 };
                            expr_index.index = Box::new(new_expr);
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(ref mut item_fn) = item {
                SliceIndexMutator.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function bodies that access slices. It modifies slice indexing operations to use an out-of-bounds index, specifically one more than the slice's maximum index. This mutation is intended to induce runtime panics due to out-of-bounds access."
    }
}