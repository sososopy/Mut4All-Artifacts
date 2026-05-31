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

pub struct Const_Generic_Parameter_Substitution_With_Arithmetic_Perturbation_96;

impl Mutator for Const_Generic_Parameter_Substitution_With_Arithmetic_Perturbation_96 {
    fn name(&self) -> &str {
        "Const_Generic_Parameter_Substitution_With_Arithmetic_Perturbation_96"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut const_param_name: Option<Ident> = None;
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param_name = Some(const_param.ident.clone());
                        break;
                    }
                }
                if const_param_name.is_none() {
                    const_param_name = Some(Ident::new("N", Span::call_site()));
                    func.sig.generics.params.push(parse_quote!(const N: usize = 1));
                }
                let param_name = const_param_name.unwrap();
                let perturbation = if rng.gen_bool(0.5) { 1 } else { 2 };
                let operation = if rng.gen_bool(0.5) { "+" } else { "-" };
                let new_expr: Expr = if operation == "+" {
                    parse_quote!((#param_name + #perturbation))
                } else {
                    parse_quote!((#param_name - #perturbation))
                };
                struct ConstParamVisitor<'a> {
                    param_name: &'a Ident,
                    new_expr: &'a Expr,
                }
                impl<'a> VisitMut for ConstParamVisitor<'a> {
                    fn visit_expr_mut(&mut self, expr: &mut Expr) {
                        if let Expr::Path(ExprPath { path, .. }) = expr {
                            if path.segments.len() == 1 && path.segments[0].ident == *self.param_name {
                                *expr = self.new_expr.clone();
                            }
                        }
                        syn::visit_mut::visit_expr_mut(self, expr);
                    }
                }
                let mut visitor = ConstParamVisitor {
                    param_name: &param1_name,
                    new_expr: &new_expr,
                };
                visitor.visit_block_mut(&mut func.block);
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut const_param_name: Option<Ident> = None;
                        for param in &func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                const_param_name = Some(const_param.ident.clone());
                                break;
                            }
                        }
                        if const_param_name.is_none() {
                            const_param_name = Some(Ident::new("N", Span::call_site()));
                            func.sig.generics.params.push(parse_quote!(const N: usize = 1));
                        }
                        let param_name = const_param_name.unwrap();
                        let perturbation = if rng.gen_bool(0.5) { 1 } else { 2 };
                        let operation = if rng.gen_bool(0.5) { "+" } else { "-" };
                        let new_expr: Expr = if operation == "+" {
                            parse_quote!((#param_name + #perturbation))
                        } else {
                            parse_quote!((#param_name - #perturbation))
                        };
                        struct ConstParamVisitor<'a> {
                            param_name: &'a Ident,
                            new_expr: &'a Expr,
                        }
                        impl<'a> VisitMut for ConstParamVisitor<'a> {
                            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                                if let Expr::Path(ExprPath { path, .. }) = expr {
                                    if path.segments.len() == 1 && path.segments[0].ident == *self.param_name {
                                        *expr = self.new_expr.clone();
                                    }
                                }
                                syn::visit_mut::visit_expr_mut(self, expr);
                            }
                        }
                        let mut visitor = ConstParamVisitor {
                            param_name: &param_name,
                            new_expr: &new_expr,
                        };
                        visitor.visit_block_mut(&mut func.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters used within function bodies. It first ensures a const generic parameter exists by adding one if missing. Then, it replaces every occurrence of that parameter's identifier in expressions with an arithmetic perturbation (addition or subtraction of 1 or 2), enclosed in parentheses to maintain precedence. This transformation modifies array lengths, slice ranges, and arithmetic operations involving const generics, potentially exposing bugs in const evaluation, type unification, or generic inference during compilation."
    }
}