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

pub struct Replace_Opaque_Return_With_Unit_162;

impl Mutator for Replace_Opaque_Return_With_Unit_162 {
    fn name(&self) -> &str {
        "Replace_Opaque_Return_With_Unit_162"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = OpaqueReturnVisitor::new();
        visitor.visit_file_mut(file);
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        item_fn.sig.output = ReturnType::Default;
                        let mut new_block = item_fn.block.clone();
                        let last_stmt = new_block.stmts.last();
                        if let Some(Stmt::Expr(expr, None)) = last_stmt {
                            if let Expr::Return(_) = expr {
                                new_block.stmts.pop();
                                new_block.stmts.push(Stmt::Expr(parse_quote! { () }, None));
                            } else {
                                new_block.stmts.push(Stmt::Expr(parse_quote! { () }, None));
                            }
                        } else {
                            new_block.stmts.push(Stmt::Expr(parse_quote! { () }, None));
                        }
                        item_fn.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait`. It replaces the return type with `()` and adjusts the function body to return `()`. At call sites, it removes or adapts usage of the return value to maintain type compatibility. This transformation tests the compiler's handling of opaque return type substitution, type inference at call sites, and the interplay between function signatures and body expressions."
    }
}

struct OpaqueReturnVisitor {
    call_sites: Vec<ExprCall>,
}

impl OpaqueReturnVisitor {
    fn new() -> Self {
        Self { call_sites: Vec::new() }
    }
}

impl VisitMut for OpaqueReturnVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        self.call_sites.push(node.clone());
    }
}