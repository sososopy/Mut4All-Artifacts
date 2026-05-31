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

pub struct Replace_Generic_Const_Type_With_Float_393;

impl Mutator for Replace_Generic_Const_Type_With_Float_393 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Type_With_Float_393"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let mut const_params = impl_item.generics.params.iter_mut()
                    .filter_map(|param| match param {
                        GenericParam::Const(param) => Some(param),
                        _ => None,
                    })
                    .collect::<Vec<_>>();
                if !const_params.is_empty() {
                    let param = const_params[0];
                    param.ty = parse_quote!(f64);
                    let ident = param.ident.clone();
                    let replacement_expr = parse_quote! { #ident as u32 };
                    let mut visitor = ReplaceConstExprVisitor {
                        target_ident: ident,
                        replacement: replacement_expr,
                    };
                    visitor.visit_type_mut(&mut impl_item.self_ty);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms generic const parameters in impl blocks from integer types to f64, then replaces their usage in const expressions with integer casts. This creates invalid float-to-integer conversions in const contexts where only integers are permitted, forcing the compiler to handle type mismatches in const evaluation logic."
    }
}

struct ReplaceConstExprVisitor {
    target_ident: Ident,
    replacement: Box<Expr>,
}

impl VisitMut for ReplaceConstExprVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if expr_path.path.is_ident(&self.target_ident) {
                *expr = *self.replacement.clone();
                return;
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}