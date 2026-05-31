use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Generics, Item,
    ItemFn, ItemStruct, ItemEnum, ItemUnion, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Generic_Closure_Default_With_Zero_49;

impl Mutator for Replace_Const_Generic_Closure_Default_With_Zero_49 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Closure_Default_With_Zero_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                self.mutate_generics(&mut item_struct.generics);
            }
            if let Item::Enum(item_enum) = item {
                self.mutate_generics(&mut item_enum.generics);
            }
            if let Item::Union(item_union) = item {
                self.mutate_generics(&mut item_union.generics);
            }
            if let Item::Trait(item_trait) = item {
                self.mutate_generics(&mut item_trait.generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct, enum, union, and trait definitions with generic const parameters that have closure default expressions. It replaces the closure default with a zero-initialized cast to the parameter's type (e.g., `{ 0 as fn() }`). This transformation aims to stress the compiler's const evaluation system by introducing zero values for function pointer types, potentially exposing internal errors during monomorphization or valtree conversion when non-valtree-compatible types are used in generic const defaults."
    }
}

impl Replace_Const_Generic_Closure_Default_With_Zero_49 {
    fn mutate_generics(&self, generics: &mut Generics) {
        for param in &mut generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default) = &mut const_param.default {
                    if self.is_closure_expression(default) && self.is_function_like_type(&const_param.ty) {
                        let ty = const_param.ty.clone();
                        let zero_expr: Expr = parse_quote! { 0 };
                        let cast_expr: Expr = parse_quote! { (#zero_expr as #ty) };
                        let block_expr: Expr = parse_quote! { { #cast_expr } };
                        *default = block_expr;
                    }
                }
            }
        }
    }

    fn is_closure_expression(&self, expr: &Expr) -> bool {
        match expr {
            Expr::Closure(_) => true,
            Expr::Block(block_expr) => {
                if block_expr.block.stmts.len() == 1 {
                    match &block_expr.block.stmts[0] {
                        Stmt::Expr(inner_expr, _) => self.is_closure_expression(&inner_expr),
                        _ => false,
                    }
                } else {
                    false
                }
            }
            _ => false,
        }
    }

    fn is_function_like_type(&self, ty: &Type) -> bool {
        match ty {
            Type::Path(type_path) => {
                let segments = &type_path.path.segments;
                if segments.len() == —1 {
                    let segment = &segments[0];
                    let ident_str = segment.ident.to_string();
                    ident_str.starts_with("fn") || ident_str == "Fn" || ident_str == "FnMut" || ident_str == "FnOnce"
                } else {
                    false
                }
            }
            _ => false,
        }
    }
}