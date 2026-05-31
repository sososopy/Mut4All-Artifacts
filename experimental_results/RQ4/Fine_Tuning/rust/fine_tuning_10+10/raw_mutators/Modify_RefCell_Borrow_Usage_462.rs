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

pub struct Modify_RefCell_Borrow_Usage_462;

impl Mutator for Modify_RefCell_Borrow_Usage_462 {
    fn name(&self) -> &str {
        "Modify_RefCell_Borrow_Usage_462"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct RefCellVisitor;

        impl VisitMut for RefCellVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::MethodCall(method_call) = expr {
                    if method_call.method == "borrow_mut" {
                        method_call.method = Ident::new("borrow", method_call.method.span());
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut visitor = RefCellVisitor;
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets instances where `RefCell` is used and `borrow_mut()` is called. It replaces `borrow_mut()` with `borrow()`, changing the mutable borrow to an immutable one. This transformation challenges the borrow checker, potentially leading to conflicts if mutable borrows are expected. It tests the compiler's handling of borrow scopes, particularly under the influence of Non-Lexical Lifetimes (NLL)."
    }
}