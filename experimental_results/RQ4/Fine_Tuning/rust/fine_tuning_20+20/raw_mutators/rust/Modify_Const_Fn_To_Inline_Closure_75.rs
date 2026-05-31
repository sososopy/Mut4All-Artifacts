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

pub struct Modify_Const_Fn_To_Inline_Closure_75;

impl Mutator for Modify_Const_Fn_To_Inline_Closure_75 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_To_Inline_Closure_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_fn_map = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Type::BareFn(type_bare_fn) = &*item_const.ty {
                    if type_bare_fn.lifetimes.is_none()
                        && type_bare_fn.inputs.is_empty()
                        && type_bare_fn.variadic.is_none()
                        && type_bare_fn.unsafety.is_none()
                        && type_bare_fn.abi.is_none()
                    {
                        const_fn_map.insert(item_const.ident.to_string());
                    }
                }
            }
        }
        file.items.retain(|item| {
            if let Item::Const(item_const) = item {
                !const_fn_map.contains(&item_const.ident.to_string())
            } else {
                true
            }
        });
        struct ConstFnReplacer<'a> {
            const_fn_map: &'a HashSet<String>,
        }
        impl<'a> VisitMut for ConstFnReplacer<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Reference(expr_ref) = expr {
                    if let Expr::Path(expr_path) = &*expr_ref.expr {
                        if expr_ref.mutability.is_none()
                            && expr_path.qself.is_none()
                            && expr_path.path.leading_colon.is_none()
                            && expr_path.path.segments.len() == 1
                        {
                            let ident = &expr_path.path.segments[0].ident;
                            if self.const_fn_map.contains(&ident.to_string()) {
                                *expr = parse_quote! { #ident };
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut replacer = ConstFnReplacer {
            const_fn_map: &const_fn_map,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets `const fn` declarations that return closures by inlining the closure directly at the point of use. It removes the `const fn` declaration and replaces references to it with the closure's body, ensuring the closure is explicitly written within the function that utilizes it. This transformation tests the compiler's handling of closure inlining and constant function references."
    }
}