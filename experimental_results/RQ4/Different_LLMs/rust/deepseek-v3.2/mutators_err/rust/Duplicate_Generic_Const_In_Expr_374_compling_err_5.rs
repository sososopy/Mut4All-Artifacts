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

pub struct Duplicate_Generic_Const_In_Expr_374;

impl Mutator for Duplicate_Generic_Const_In_Expr_374 {
    fn name(&self) -> &str {
        "Duplicate_Generic_Const_In_Expr_374"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DuplicateGenericConstVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions in where-clause predicates, array sizes, and similar positions that contain generic constant parameters. It identifies occurrences of generic parameters within these expressions and creates mutated versions by replacing the parameter with (parameter + parameter) or (parameter - parameter), or by appending + parameter or - parameter to the entire expression. This creates redundant arithmetic operations that stress the compiler's const evaluation system, particularly with repeated generic parameters, potentially triggering edge cases similar to the original ICE."
    }
}

struct DuplicateGenericConstVisitor {
    generic_params: HashSet<Ident>,
}

impl DuplicateGenericConstVisitor {
    fn new() -> Self {
        Self {
            generic_params: HashSet::new(),
        }
    }

    fn collect_generic_params(&mut self, generics: &Generics) {
        for param in &generics.params {
            if let GenericParam::Const(const_param) = param {
                self.generic_params.insert(const_param.ident.clone());
            }
        }
    }

    fn mutate_expr(&self, expr: &mut Expr) -> bool {
        let mut mutated = false;
        match expr {
            Expr::Binary(bin_expr) => {
                mutated |= self.mutate_expr(&mut bin_expr.left);
                mutated |= self.mutate_expr(&mut bin_expr.right);
            }
            Expr::Paren(paren_expr) => {
                mutated |= self.mutate_expr(&mut paren_expr.expr);
            }
            Expr::Group(group_expr) => {
                mutated |= self.mutate_expr(&mut group_expr.expr);
            }
            Expr::Block(block_expr) => {
                if let Some(block_expr_expr) = &mut block_expr.block.stmts.last_mut() {
                    if let Stmt::Expr(expr_stmt, _) = block_expr_expr {
                        mutated |= self.mutate_expr(&mut expr_stmt);
                    }
                }
            }
            Expr::Path(path_expr) => {
                if let Some(segment) = path_expr.path.segments.last() {
                    if self.generic_params.contains(&segment.ident) {
                        let mut rng = thread_rng();
                        let op = if rng.gen_bool(0.5) { "+" } else { "-" };
                        let new_expr: Expr = parse_quote! { (#segment #op #segment) };
                        *expr = new_expr;
                        mutated = true;
                    }
                }
            }
            _ => {}
        }
        if !mutated {
            if let Expr::Path(path_expr) = expr {
                if let Some(segment) = path_expr.path.segments.last() {
                    if self.generic_params.contains(&segment.ident) {
                        let mut rng = thread_rng();
                        let op = if rng.gen_bool(0.5) { "+" } else { "-" };
                        let new_expr: Expr = parse_quote! { (#segment #op #segment) };
                        *expr = new_expr;
                        mutated = true;
                    }
                }
            }
        }
        mutated
    }

    fn mutate_where_clause(&mut self, where_clause: &mut Option<WhereClause>) {
        if let Some(where_clause) = where_clause {
            for predicate in &mut where_clause.predicates {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Type::Array(array_type) = &mut pred_type.bounded_ty {
                        if let Expr::Const(const_expr) = &mut array_type.len {
                            let mut expr: Expr = syn::parse2(quote! { #const_expr }).unwrap();
                            if self.mutate_expr(&mut expr) {
                                array_type.len = expr;
                            }
                        } else if self.mutate_expr(&mut array_type.len) {
                            continue;
                        }
                    }
                }
            }
        }
    }

    fn mutate_type(&mut self, ty: &mut Type) {
        match ty {
            Type::Array(array_type) => {
                if let Expr::Const(const_expr) = &mut array_type.len {
                    let mut expr: Expr = syn::parse2(quote! { #const_expr }).unwrap();
                    if self.mutate_expr(&mut expr) {
                        array_type.len = expr;
                    }
                } else if self.mutate_expr(&mut array_type.len) {
                    return;
                }
            }
            Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Const(const_arg) = arg {
                                let mut expr: Expr = syn::parse2(quote! { #const_arg }).unwrap();
                                if self.mutate_expr(&mut expr) {
                                    *arg = GenericArgument::Const(expr);
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
}

impl VisitMut for DuplicateGenericConstVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        self.collect_generic_params(&i.sig.generics);
        self.mutate_where_clause(&mut i.sig.generics.where_clause);
        self.visit_block_mut(&mut i.block);
        self.generic_params.clear();
    }

    fn visit_impl_item_fn_mut(&mut self, i: &mut ImplItemFn) {
        self.collect_generic_params(&i.sig.generics);
        self.mutate_where_clause(&mut i.sig.generics.where_clause);
        self.visit_block_mut(&mut i.block);
        self.generic_params.clear();
    }

    fn visit_item_struct_mut(&mut self, i: &mut ItemStruct) {
        self.collect_generic_params(&i.generics);
        self.mutate_where_clause(&mut i.generics.where_clause);
        for field in &mut i.fields {
            self.visit_field_mut(field);
        }
        self.generic_params.clear();
    }

    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        self.collect_generic_params(&i.generics);
        self.mutate_where_clause(&mut i.generics.where_clause);
        for item in &mut i.items {
            self.visit_impl_item_mut(item);
        }
        self.generic_params.clear();
    }

    fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
        self.visit_expr_mut(&mut i.func);
        for arg in &mut i.args {
            self.visit_expr_mut(arg);
        }
    }

    fn visit_expr_array_mut(&mut self, i: &mut ExprArray) {
        for elem in &mut i.elems {
            self.visit_expr_mut(elem);
        }
    }

    fn visit_type_mut(&mut self, i: &mut Type) {
        self.mutate_type(i);
        match i {
            Type::Array(array_type) => {
                self.visit_type_mut(&mut array_type.elem);
                self.visit_expr_mut(&mut array_type.len);
            }
            Type::Path(type_path) => {
                self.visit_path_mut(&mut type_path.path);
            }
            _ => {}
        }
    }

    fn visit_field_mut(&mut self, i: &mut Field) {
        self.visit_type_mut(&mut i.ty);
    }

    fn visit_expr_mut(&mut self, i: &mut Expr) {
        self.mutate_expr(i);
        match i {
            Expr::Binary(bin_expr) => {
                self.visit_expr_mut(&mut bin_expr.left);
                self.visit_expr_mut(&mut bin_expr.right);
            }
            Expr::Paren(paren_expr) => {
                self.visit_expr_mut(&mut paren_expr.expr);
            }
            Expr::Group(group_expr) => {
                self.visit_expr_mut(&mut group_expr.expr);
            }
            Expr::Block(block_expr) => {
                self.visit_block_mut(&mut block_expr.block);
            }
            _ => {}
        }
    }
}