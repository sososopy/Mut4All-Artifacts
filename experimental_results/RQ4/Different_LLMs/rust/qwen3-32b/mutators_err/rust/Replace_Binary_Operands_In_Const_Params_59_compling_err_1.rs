use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprCast, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Binary_Operands_In_Const_Params_59;

impl Mutator for Replace_Binary_Operands_In_Const_Params_59 {
    fn name(&self) -> &str {
        "Replace_Binary_Operands_In_Const_Params_59"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(expr) = &mut const_param.default {
                            self.process_expr(expr);
                        }
                    }
                }
            }
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    for field in &mut variant.fields {
                        if let syn::Fields::Unnamed(fields) = field {
                            for field in &mut fields.unnamed {
                                if let Some(expr) = &mut field.default {
                                    self.process_expr(expr);
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(expr) = &mut const_param.default {
                            self.process_expr(expr);
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for param in &mut impl_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(expr) = &mut const_param.default {
                            self.process_expr(expr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces one operand in binary arithmetic operations within const parameter expressions with an incompatible type (e.g., changing `X + Y` to `X as i32 + Y`). This introduces type mismatches in const evaluations, potentially exposing bugs in the compiler's handling of mixed-type arithmetic during const generic resolution."
    }
}

impl Replace_Binary_Operands_In_Const_Params_59 {
    fn process_expr(&self, expr: &mut Expr) {
        let mut visitor = ReplaceBinaryOperandsVisitor;
        visitor.visit_expr_mut(expr);
    }
}

struct ReplaceBinaryOperandsVisitor;

impl ReplaceBinaryOperandsVisitor {
    fn select_random_type(&self) -> Type {
        let types = vec![
            parse_quote!(i32),
            parse_quote!(i64),
            parse_quote!(u32),
            parse_quote!(u64),
            parse_quote!(usize),
        ];
        let mut rng = thread_rng();
        let idx = rng.gen_range(0..types.len());
        types[idx].clone()
    }
}

impl VisitMut for ReplaceBinaryOperandsVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Binary(expr_binary) = expr {
            let mut rng = thread_rng();
            let operand_to_modify = if rng.gen() { &mut expr_binary.left } else { &mut expr_binary.right };
            let original_expr = operand_to_modify.clone();
            let new_type = self.select_random_type();
            let cast_expr = Expr::Cast(ExprCast {
                attrs: Vec::new(),
                expr: Box::new(original_expr),
                cast_token: token::As::default(),
                ty: Box::new(new_type),
            });
            *operand_to_modify = Box::new(cast_expr);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}