use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprClosure, ExprMethodCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Remove_Const_Context_Effect_310;

impl Mutator for Remove_Const_Context_Effect_310 {
    fn name(&self) -> &str {
        "Remove_Const_Context_Effect_310"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = ConstContextVisitor {
            rng: &mut rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const items (const, static, const blocks) containing const closures or const trait method calls. It removes the const qualifier from either the closure or the method call within the const context, creating a mismatch in const context expectations. This transformation aims to expose compiler bugs related to const effect parameter handling, potentially triggering ICEs during type-checking when the const context's requirements are violated."
    }
}

struct ConstContextVisitor<'a, R: Rng> {
    rng: &'a mut R,
    mutated: bool,
}

impl<'a, R: Rng> VisitMut for ConstContextVisitor<'a, R> {
    fn visit_item_mut(&mut self, i: &mut Item) {
        match i {
            Item::Const(item_const) => {
                self.visit_const_item(item_const);
            }
            Item::Static(item_static) => {
                self.visit_static_item(item_static);
            }
            _ => {}
        }
        if !self.mutated {
            syn::visit_mut::visit_item_mut(self, i);
        }
    }

    fn visit_expr_mut(&mut self, e: &mut Expr) {
        if self.mutated {
            return;
        }
        match e {
            Expr::Closure(expr_closure) => {
                if expr_closure.constness.is_some() {
                    if self.rng.gen_bool(0.5) {
                        expr_closure.constness = None;
                        self.mutated = true;
                        return;
                    }
                }
            }
            Expr::MethodCall(expr_method_call) => {
                if expr_method_call.constness.is_some() {
                    if self.rng.gen_bool(0.5) {
                        expr_method_call.constness = None;
                        self.mutated = true;
                        return;
                    }
                }
            }
            _ => {}
        }
        if !self.mutated {
            syn::visit_mut::visit_expr_mut(self, e);
        }
    }
}

impl<'a, R: Rng> ConstContextVisitor<'a, R> {
    fn visit_const_item(&mut self, item_const: &mut ItemConst) {
        self.visit_expr_mut(&mut item_const.expr);
    }

    fn visit_static_item(&mut self, item_static: &mut ItemStatic) {
        self.visit_expr_mut(&mut item_static.expr);
    }
}