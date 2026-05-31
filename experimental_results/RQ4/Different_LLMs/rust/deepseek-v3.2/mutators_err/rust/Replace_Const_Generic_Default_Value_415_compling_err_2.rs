use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Generic_Default_Value_415;

impl Mutator for Replace_Const_Generic_Default_Value_415 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Default_Value_415"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_exprs = Vec::new();
        let mut const_types = Vec::new();
        let mut rng = thread_rng();

        struct ConstCollector<'a> {
            exprs: &'a mut Vec<Expr>,
            types: &'a mut Vec<Type>,
        }
        impl<'a> Visit<'a> for ConstCollector<'a> {
            fn visit_item_const(&mut self, node: &'a ItemConst) {
                self.exprs.push(*node.expr.clone());
                self.types.push(*node.ty.clone());
                syn::visit::visit_item_const(self, node);
            }
            fn visit_expr_lit(&mut self, node: &'a ExprLit) {
                self.exprs.push(Expr::Lit(node.clone()));
                syn::visit::visit_expr_lit(self, node);
            }
            fn visit_expr_path(&mut self, node: &'a ExprPath) {
                self.exprs.push(Expr::Path(node.clone()));
                syn::visit::visit_expr_path(self, node);
            }
        }
        let mut collector = ConstCollector {
            exprs: &mut const_exprs,
            types: &mut const_types,
        };
        collector.visit_file(file);

        if const_exprs.is_empty() {
            return;
        }

        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    for param in &mut item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &mut const_param.default {
                                let param_type = const_param.ty.clone();
                                let mut candidates = Vec::new();
                                for (expr, ty) in const_exprs.iter().zip(const_types.iter()) {
                                    if ty == &param_type {
                                        candidates.push(expr.clone());
                                    }
                                }
                                if !candidates.is_empty() {
                                    let new_expr = candidates.choose(&mut rng).unwrap().clone();
                                    if new_expr != *default {
                                        *default = new_expr;
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    for param in &mut item_enum.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &mut const_param.default {
                                let param_type = const_param.ty.clone();
                                let mut candidates = Vec::new();
                                for (expr, ty) in const_exprs.iter().zip(const_types.iter()) {
                                    if ty == &param_type {
                                        candidates.push(expr.clone());
                                    }
                                }
                                if !candidates.is_empty() {
                                    let new_expr = candidates.choose(&mut rng).unwrap().clone();
                                    if new_expr != *default {
                                        *default = new_expr;
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    for param in &mut item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(default) = &mut const_param.default {
                                let param_type = const_param.ty.clone();
                                let mut candidates = Vec::new();
                                for (expr, ty) in const_exprs.iter().zip(const_types.iter()) {
                                    if ty == &param_type {
                                        candidates.push(expr.clone());
                                    }
                                }
                                if !candidates.is_empty() {
                                    let new_expr = candidates.choose(&mut rng).unwrap().clone();
                                    if new_expr != *default {
                                        *default = new_expr;
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces default values of const generic parameters in struct, enum, and function definitions with alternative const expressions of the same type found elsewhere in the seed program. It first collects all const expressions and their types from the entire file, including const items and literals. Then it iterates through definitions, identifying const generic parameters with default values. For each such parameter, it selects a random const expression of matching type from the collected pool, ensuring it differs from the original default. This transformation tests the compiler's const evaluation, type consistency, and default substitution logic in generic contexts, potentially triggering subtle bugs in const generic instantiation and default value resolution."
    }
}