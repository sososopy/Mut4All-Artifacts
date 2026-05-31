use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprCall, ExprPath, File, FnArg, GenericArgument,
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct AsymmetricConstOp_394;

impl Mutator for AsymmetricConstOp_394 {
    fn name(&self) -> &str {
        "AsymmetricConstOp_394"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let const_params = collect_const_params(&item_impl.generics);
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut visitor = BinaryExprVisitor {
                            const_params: const_params.clone(),
                        };
                        visitor.visit_block_mut(&mut func.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets arithmetic operations in generic const expressions that combine parameters of different numeric types (e.g., i64 and usize). It modifies existing expressions by introducing asymmetric operations (e.g., B * 2) that force implicit type coercion, testing the compiler's handling of type conversion in const evaluation contexts and potentially exposing bugs in type coercion logic or const evaluation paths."
    }
}

fn collect_const_params(generics: &syn::Generics) -> Vec<(Ident, Type)> {
    generics
        .params
        .iter()
        .filter_map(|param| {
            if let GenericParam::Const(const_param) = param {
                Some((
                    const_param.ident.clone(),
                    const_param.ty.clone(),
                ))
            } else {
                None
            }
        })
        .collect()
}

struct BinaryExprVisitor {
    const_params: Vec<(Ident, Type)>,
}

impl BinaryExprVisitor {
    fn is_const_param_ref(&self, expr: &Expr) -> bool {
        if let Expr::Path(expr_path) = expr {
            let ident = &expr_path.path.segments[0].ident;
            self.const_params.iter().any(|(name, _)| name == ident)
        } else {
            false
        }
    }

    fn get_const_param_type(&self, expr: &Expr) -> Option<Type> {
        if let Expr::Path(expr_path) = expr {
            let ident = &expr_path.path.segments[0].ident;
            self.const_params
                .iter()
                .find(|(name, _)| name == ident)
                .map(|(_, ty)| ty.clone())
        } else {
            None
        }
    }

    fn get_type_name(ty: &Type) -> Option<String> {
        if let Type::Path(type_path) = ty {
            if let Some(segment) = type_path.path.segments.iter().next() {
                Some(segment.ident.to_string())
            } else {
                None
            }
        } else {
            None
        }
    }
}

impl VisitMut for BinaryExprVisitor {
    fn visit_expr_binary_mut(&mut self, expr: &mut ExprBinary) {
        let left_is_const = self.is_const_param_ref(&expr.left);
        let right_is_const = self.is_const_param_ref(&expr.right);

        if left_is_const && right_is_const {
            if let Some(left_type) = self.get_const_param_type(&expr.left) {
                if let Some(right_type) = self.get_const_param_type(&expr.right) {
                    let left_type_name = Self::get_type_name(&left_type);
                    let right_type_name = Self::get_type_name(&right_type);
                    if left_type_name != right_type_name {
                        let new_right = parse_quote! { #expr.right * 2 };
                        expr.right = new_right;
                    }
                }
            }
        }

        syn::visit_mut::visit_expr_binary_mut(self, expr);
    }
}