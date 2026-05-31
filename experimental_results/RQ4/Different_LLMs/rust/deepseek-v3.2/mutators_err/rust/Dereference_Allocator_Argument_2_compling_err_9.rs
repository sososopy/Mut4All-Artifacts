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

pub struct Dereference_Allocator_Argument_2;

impl Mutator for Dereference_Allocator_Argument_2 {
    fn name(&self) -> &str {
        "Dereference_Allocator_Argument_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AllocatorDerefVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets allocator-aware functions (e.g., `new_in`, `with_capacity_in`) that take an allocator by reference. It dereferences the allocator argument, changing its type from `&A` to `A`. This transformation may expose bugs in the compiler's layout computation or type handling for allocator types, as the function now receives the allocator by value instead of by reference."
    }
}

struct AllocatorDerefVisitor;

impl VisitMut for AllocatorDerefVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            let last_segment = path.segments.last();
            if let Some(segment) = last_segment {
                let func_name = segment.ident.to_string();
                if func_name.ends_with("_in") {
                    if let Some(last_arg) = node.args.last_mut() {
                        if is_reference_expression(last_arg) {
                            let deref_expr: Expr = parse_quote! { *#last_arg };
                            *last_arg = deref_expr;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}

fn is_reference_expression(expr: &Expr) -> bool {
    match expr {
        Expr::Reference(_) => true,
        Expr::Path(ExprPath { path, .. }) => {
            let ty_str = quote!(#path).to_string();
            ty_str.starts_with('&') || ty_str.contains("&mut")
        }
        _ => false,
    }
}