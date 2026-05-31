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

pub struct Const_Expression_Mutation_84;

impl Mutator for Const_Expression_Mutation_84 {
    fn name(&self) -> &str {
        "Const_Expression_Mutation_84"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let const_params: HashSet<String> = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.ident.to_string())
                        } else {
                            None
                        }
                    })
                    .collect();

                let mut visitor = ConstExprVisitor {
                    const_params: &const_params,
                    mutated: false,
                };

                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const generic parameters and modifies their const expressions, particularly those involving arithmetic operations with const generics, to test the compiler's handling of const expression variations."
    }
}

struct ConstExprVisitor<'a> {
    const_params: &'a HashSet<String>,
    mutated: bool,
}

impl<'a> VisitMut for ConstExprVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if self.mutated {
            return;
        }

        if let Expr::Binary(expr_binary) = expr {
            if let Expr::Path(expr_path) = &*expr_binary.left {
                if let Some(ident) = expr_path.path.get_ident() {
                    if self.const_params.contains(&ident.to_string()) {
                        let new_op = match expr_binary.op {
                            BinOp::Add(_) => BinOp::Sub(Default::default()),
                            BinOp::Sub(_) => BinOp::Add(Default::default()),
                            BinOp::Mul(_) => BinOp::Div(Default::default()),
                            BinOp::Div(_) => BinOp::Mul(Default::default()),
                            _ => return,
                        };
                        expr_binary.op = new_op;
                        self.mutated = true;
                    }
                }
            }
        }

        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if self.mutated {
            return;
        }

        if let Type::Array(type_array) = ty {
            if let Expr::Binary(expr_binary) = &mut type_array.len {
                if let Expr::Path(expr_path) = &*expr_binary.left {
                    if let Some(ident) = expr_path.path.get_ident() {
                        if self.const_params.contains(&ident.to_string()) {
                            let new_op = match expr_binary.op {
                                BinOp::Add(_) => BinOp::Sub(Default::default()),
                                BinOp::Sub(_) => BinOp::Add(Default::default()),
                                BinOp::Mul(_) => BinOp::Div(Default::default()),
                                BinOp::Div(_) => BinOp::Mul(Default::default()),
                                _ => return,
                            };
                            expr_binary.op = new_op;
                            self.mutated = true;
                        }
                    }
                }
            }
        }

        syn::visit_mut::visit_type_mut(self, ty);
    }
}