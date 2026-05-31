use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
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

pub struct MacroWrapConstGeneric_110;

impl Mutator for MacroWrapConstGeneric_110 {
    fn name(&self) -> &str {
        "MacroWrapConstGeneric_110"
    }
    fn mutate(&self, file: &mut syn::File) {
        self.add_wrap_macro(file);
        let mut visitor = WrapConstGenericVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a macro `wrap!` and replaces direct expressions in const generic contexts with `wrap!(expression)`. This tests the compiler's handling of macro expansion within const parameters, potentially exposing issues in macro hygiene, const evaluation, or nested macro resolution."
    }
}

impl MacroWrapConstGeneric_110 {
    fn add_wrap_macro(&self, file: &mut syn::File) {
        let has_wrap = file.items.iter().any(|item| {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("macro_rules") {
                    let tokens_str = mac.mac.tokens.to_string();
                    if tokens_str.starts_with("wrap!") {
                        return true;
                    }
                }
                false
            } else {
                false
            }
        });

        if !has_wrap {
            let wrap_macro: ItemMacro = parse_quote! {
                macro_rules! wrap {
                    ($v:expr) => { $v };
                }
            };
            file.items.insert(0, Item::Macro(wrap_macro));
        }
    }
}

struct WrapConstGenericVisitor;

impl VisitMut for WrapConstGenericVisitor {
    fn visit_type_array_mut(&mut self, ty: &mut TypeArray) {
        let len_expr = ty.len.clone();
        let wrap_call = parse_quote! { wrap!(#len_expr) };
        ty.len = wrap_call;
        syn::visit_mut::visit_type_array_mut(self, ty);
    }

    fn visit_type_path_mut(&mut self, i: &mut TypePath) {
        for segment in &mut i.path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        let wrap_call = parse_quote! { wrap!(#expr) };
                        *expr = wrap_call;
                    }
                }
            }
        }
        syn::visit_mut::visit_type_path_mut(self, i);
    }

    fn visit_generic_param_mut(&mut self, i: &mut GenericParam) {
        if let GenericParam::Const(param) = i {
            if let Some(expr) = &mut param.default {
                let wrap_call = parse_quote! { wrap!(#expr) };
                *expr = wrap_call;
            }
        }
        syn::visit_mut::visit_generic_param_mut(self, i);
    }
}