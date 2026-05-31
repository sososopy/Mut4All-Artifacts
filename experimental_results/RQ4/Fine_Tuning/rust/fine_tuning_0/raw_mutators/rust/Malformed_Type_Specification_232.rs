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

pub struct Malformed_Type_Specification_232;

impl Mutator for Malformed_Type_Specification_232 {
    fn name(&self) -> &str {
        "Malformed_Type_Specification_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = TypePathVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function call expressions with type specifications using '::'. It replaces valid type paths with malformed constructs to introduce parsing errors, testing the compiler's resilience to unexpected token sequences in type specifications."
    }
}

struct TypePathVisitor;

impl VisitMut for TypePathVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if path.segments.len() > 1 {
                let malformed_expr: Expr = parse_quote! { Layout::(x: !) };
                node.func = Box::new(malformed_expr);
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}