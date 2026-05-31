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

pub struct Replace_Const_Generic_Arg_With_Outer_Param_345;

impl Mutator for Replace_Const_Generic_Arg_With_Outer_Param_345 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Arg_With_Outer_Param_345"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut outer_const_params = Vec::new();
        let mut visitor = OuterConstParamVisitor {
            params: &mut outer_const_params,
            current_scope: Vec::new(),
        };
        visitor.visit_file(&file);
        if outer_const_params.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let mut visitor_mut = ConstGenericArgVisitorMut {
            outer_params: &outer_const_params,
            rng: &mut rng,
            changed: false,
        };
        visitor_mut.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies constant generic arguments in function calls, struct instantiations, or type annotations. It replaces the original constant expression with a new expression referencing a const generic parameter from an outer scope that is not accessible in the current context. This transformation creates syntactically valid but semantically incorrect code, aiming to trigger compiler ICEs during well-formedness checking of const generic expressions, particularly when using the generic_const_exprs feature."
    }
}

struct OuterConstParamVisitor<'a> {
    params: &'a mut Vec<(Ident, usize)>,
    current_scope: Vec<usize>,
}

impl<'a> Visit<'a> for OuterConstParamVisitor<'a> {
    fn visit_item_fn(&mut self, i: &'a ItemFn) {
        let scope_id = self.params.len();
        self.current_scope.push(scope_id);
        for param in &i.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                self.params.push((const_param.ident.clone(), scope_id));
            }
        }
        syn::visit::visit_item_fn(self, i);
        self.current_scope.pop();
    }
    fn visit_item_struct(&mut self, i: &'a ItemStruct) {
        let scope_id = self.params.len();
        self.current_scope.push(scope_id);
        for param in &i.generics.params {
            if let GenericParam::Const(const_param) = param {
                self.params.push((const_param.ident.clone(), scope_id));
            }
        }
        syn::visit::visit_item_struct(self, i);
        self.current_scope.pop();
    }
    fn visit_impl_item_fn(&mut self, i: &'a syn::ImplItemFn) {
        let scope_id = self.params.len();
        self.current_scope.push(scope_id);
        for param in &i.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                self.params.push((const_param.ident.clone(), scope_id));
            }
        }
        syn::visit::visit_impl_item_fn(self, i);
        self.current_scope.pop();
    }
}

struct ConstGenericArgVisitorMut<'a, R: Rng> {
    outer_params: &'a Vec<(Ident, usize)>,
    rng: &'a mut R,
    changed: bool,
}

impl<'a, R: Rng> VisitMut for ConstGenericArgVisitorMut<'a, R> {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Call(ExprCall { func, args }) = node {
            if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                self.visit_path_mut(path);
            }
            for arg in args.iter_mut() {
                self.visit_expr_mut(arg);
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(TypePath { path, .. }) = node {
            self.visit_path_mut(path);
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
    fn visit_path_mut(&mut self, node: &mut SynPath) {
        for segment in &mut node.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        self.mutate_const_expr(expr);
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, node);
    }
}

impl<'a, R: Rng> ConstGenericArgVisitorMut<'a, R> {
    fn mutate_const_expr(&mut self, expr: &mut Expr) {
        if self.changed {
            return;
        }
        let mut current_scope_consts = HashSet::new();
        let mut visitor = CurrentScopeConstVisitor {
            consts: &mut current_scope_consts,
        };
        visitor.visit_expr(expr);
        let candidate_params: Vec<&(Ident, usize)> = self.outer_params
            .iter()
            .filter(|(ident, _)| !current_scope_consts.contains(ident))
            .collect();
        if candidate_params.is_empty() {
            return;
        }
        let (selected_ident, _) = candidate_params.choose(self.rng).unwrap();
        let new_expr: Expr = parse_quote! { #selected_ident };
        *expr = new_expr;
        self.changed = true;
    }
}

struct CurrentScopeConstVisitor<'a> {
    consts: &'a mut HashSet<Ident>,
}

impl<'a> Visit<'a> for CurrentScopeConstVisitor<'a> {
    fn visit_ident(&mut self, i: &'a Ident) {
        self.consts.insert(i.clone());
    }
}