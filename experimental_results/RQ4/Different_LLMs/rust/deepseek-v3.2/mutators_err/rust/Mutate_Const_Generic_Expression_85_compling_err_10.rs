use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BinOp, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Mutate_Const_Generic_Expression_85;

impl Mutator for Mutate_Const_Generic_Expression_85 {
    fn name(&self) -> &str {
        "Mutate_Const_Generic_Expression_85"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstGenericExpressionVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets arithmetic expressions within const generic arguments, such as `{ N + 1 }`. It cycles through operators (+,-,*,/,%) in a deterministic sequence, replacing the original operator. This transformation tests edge cases in const expression evaluation and inference, potentially triggering ICEs in the Rust compiler, especially with the generic_const_exprs feature."
    }
}

struct ConstGenericExpressionVisitor;

impl ConstGenericExpressionVisitor {
    fn new() -> Self {
        Self
    }

    fn mutate_operator(&self, op: &mut BinOp) {
        let operators = [
            BinOp::Add(<token::Plus>::default()),
            BinOp::Sub(<token::Minus>::default()),
            BinOp::Mul(<token::Star>::default()),
            BinOp::Div(<token::Slash>::default()),
            BinOp::Rem(<token::Percent>::default()),
        ];
        let current_idx = operators.iter().position(|o| std::mem::discriminant(o) == std::mem::discriminant(op)).unwrap_or(0);
        let next_idx = (current_idx + 1) % operators.len();
        *op = operators[next_idx];
    }

    fn is_const_generic_expression(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Binary(binary) => {
                let left_valid = self.is_const_generic_param_or_literal(&binary.left);
                let right_valid = self.is_const_generic_param_or_literal(&binary.right);
                left_valid && right_valid
            }
            _ => false,
        }
    }

    fn is_const_generic_param_or_literal(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Path(path) => {
                path.path.segments.len() == 1 && path.path.segments[0].arguments.is_none()
            }
            Expr::Lit(lit) => match &lit.lit {
                Lit::Int(_) => true,
                _ => false,
            },
            _ => false,
        }
    }
}

impl VisitMut for ConstGenericExpressionVisitor {
    fn visit_generic_argument_mut(&mut self, arg: &mut GenericArgument) {
        if let GenericArgument::Const(expr) = arg {
            if self.is_const_generic_expression(expr) {
                if let Expr::Binary(binary) = expr {
                    self.mutate_operator(&mut binary.op);
                }
            }
        }
        syn::visit_mut::visit_generic_argument_mut(self, arg);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        self.visit_generic_argument_mut(arg);
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Struct(struct_item) => {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if self.is_const_generic_expression(default) {
                                if let Expr::Binary(binary) = default {
                                    self.mutate_operator(&mut binary.op);
                                }
                            }
                        }
                    }
                }
            }
            Item::Fn(func_item) => {
                for param in &mut func_item.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if self.is_const_generic_expression(default) {
                                if let Expr::Binary(binary) = default {
                                    self.mutate_operator(&mut binary.op);
                                }
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut func_item.sig.output {
                    self.visit_type_mut(return_type);
                }
            }
            Item::Impl(impl_item) => {
                for param in &mut impl_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if self.is_const_generic_expression(default) {
                                if let Expr::Binary(binary) = default {
                                    self.mutate_operator(&mut binary.op);
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for param in &mut method.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Some(default) = &mut const_param.default {
                                    if self.is_const_generic_expression(default) {
                                        if let Expr::Binary(binary) = default {
                                            self.mutate_operator(&mut binary.op);
                                        }
                                    }
                                }
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            self.visit_type_mut(return_type);
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, item);
    }
}