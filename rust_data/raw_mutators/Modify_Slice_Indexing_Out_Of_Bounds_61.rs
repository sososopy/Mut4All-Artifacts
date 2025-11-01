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

pub struct Modify_Slice_Indexing_Out_Of_Bounds_61;

impl Mutator for Modify_Slice_Indexing_Out_Of_Bounds_61 {
    fn name(&self) -> &str {
        "Modify_Slice_Indexing_Out_Of_Bounds_61"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct IndexMutator;
        
        impl VisitMut for IndexMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(expr_index) = expr {
                    if let Expr::Path(ref path) = *expr_index.expr {
                        if path.path.segments.last().unwrap().ident == "buffer" {
                            let len_expr: Expr = parse_quote! { self.buffer.len() };
                            let out_of_bounds_expr: Expr = parse_quote! { self.buffer[self.buffer.len()..self.buffer.len() + 1] };
                            *expr = out_of_bounds_expr;
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        
        let mut visitor = IndexMutator;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies instances of slice or vector indexing within function bodies. It modifies these instances to access elements out of bounds by using an index greater than the length of the array or vector. This is achieved by replacing the current slice with an expression that intentionally goes out of bounds, such as using the length of the vector as both the start and end of the slice, plus one."
    }
}