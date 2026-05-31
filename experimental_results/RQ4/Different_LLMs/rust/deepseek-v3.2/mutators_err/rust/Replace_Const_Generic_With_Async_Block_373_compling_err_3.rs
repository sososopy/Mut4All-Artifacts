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

pub struct Replace_Const_Generic_With_Async_Block_373;

impl Mutator for Replace_Const_Generic_With_Async_Block_373 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Async_Block_373"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceConstGenericVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments in function signatures and call sites. It replaces integer literal expressions with async blocks that return the same integer value, wrapped in curly braces as required for const generic arguments. This transformation aims to trigger type confusion between async blocks and closures within const evaluation, mimicking bug scenarios where an async block appears in a const generic argument context, potentially leading to internal compiler errors about expected closure or coroutine types."
    }
}

struct ReplaceConstGenericVisitor;

impl VisitMut for ReplaceConstGenericVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Lit(expr_lit) = expr {
            if let syn::Lit::Int(_) = &expr_lit.lit {
                let parent = std::mem::replace(expr, Expr::Verbatim(quote! {}));
                let new_expr: Expr = parse_quote!({ async { #parent } });
                *expr = new_expr;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_generic_param_mut(&mut self, param: &mut GenericParam) {
        if let GenericParam::Const(const_param) = param {
            if let Some(default) = &mut const_param.default {
                self.visit_expr_mut(default);
            }
        }
        syn::visit_mut::visit_generic_param_mut(self, param);
    }

    fn visit_path_arguments_mut(&mut self, args: &mut PathArguments) {
        if let PathArguments::AngleBracketed(angle_args) = args {
            for arg in &mut angle_args.args {
                if let GenericArgument::Const(const_expr) = arg {
                    self.visit_expr_mut(const_expr);
                }
            }
        }
        syn::visit_mut::visit_path_arguments_mut(self, args);
    }
}