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

pub struct Mutable_Static_To_UnsafeCell_4;

impl Mutator for Mutable_Static_To_UnsafeCell_4 {
    fn name(&self) -> &str {
        "Mutable_Static_To_UnsafeCell_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if item_static.mutability.is_some() {
                    let ty = &item_static.ty;
                    let expr = &item_static.expr;
                    let new_ty: Type = parse_quote!(std::cell::UnsafeCell<#ty>);
                    let new_expr: Expr = parse_quote!(std::cell::UnsafeCell::new(#expr));
                    item_static.mutability = None;
                    item_static.ty = Box::new(new_ty);
                    item_static.expr = Box::new(new_expr);
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = StaticAccessVisitor::new();
                visitor.visit_block_mut(&mut item_fn.block);

                for (ident, span) in visitor.static_accesses {
                    let new_expr: Expr = parse_quote!({
                        let ptr = #ident.get();
                        unsafe { &mut *ptr }
                    });
                    let stmt = Stmt::Expr(new_expr);
                    item_fn.block.stmts.insert(0, stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets mutable static variables, wrapping them in `UnsafeCell` to introduce interior mutability. It adjusts references to these variables, ensuring accesses are done through `UnsafeCell::get()`. This transformation tests the handling of mutable statics and interior mutability, potentially exposing concurrency issues or unsafe access patterns."
    }
}

struct StaticAccessVisitor {
    static_accesses: Vec<(Ident, Span)>,
}

impl StaticAccessVisitor {
    fn new() -> Self {
        StaticAccessVisitor {
            static_accesses: Vec::new(),
        }
    }
}

impl VisitMut for StaticAccessVisitor {
    fn visit_expr_path_mut(&mut self, i: &mut ExprPath) {
        if let Some(ident) = i.path.get_ident() {
            self.static_accesses.push((ident.clone(), ident.span()));
        }
        visit_mut::visit_expr_path_mut(self, i);
    }
}