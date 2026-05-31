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

pub struct Replace_Const_With_Empty_Array_129;

impl Mutator for Replace_Const_With_Empty_Array_129 {
    fn name(&self) -> &str {
        "Replace_Const_With_Empty_Array_129"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if item_const.ty.is_none() {
                    let init_expr = &item_const.expr;
                    if !is_empty_array_literal(init_expr) {
                        item_const.expr = parse_quote! { [] };
                    }
                }
            }
        }
        let mut visitor = ConstVisitor {
            mutator: self,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const declarations without explicit type annotations and replaces their initializer expressions with an empty array literal `[]`. This creates ambiguous type contexts because the empty array lacks explicit length and element type information, potentially triggering type inference failures or drop elaboration bugs. The operator mimics a reported compiler bug where an empty array in a const without a type caused a delayed span bug panic."
    }
}

struct ConstVisitor<'a> {
    mutator: &'a Replace_Const_With_Empty_Array_129,
}

impl<'a> VisitMut for ConstVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Array(expr_array) = expr {
            if expr_array.elems.is_empty() {
                return;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn is_empty_array_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Array(expr_array) => expr_array.elems.is_empty(),
        _ => false,
    }
}