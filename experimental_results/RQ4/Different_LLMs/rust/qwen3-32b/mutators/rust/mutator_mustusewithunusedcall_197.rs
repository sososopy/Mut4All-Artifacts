use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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

pub struct Mutator_MustUseWithUnusedCall_197;

impl Mutator for Mutator_MustUseWithUnusedCall_197 {
    fn name(&self) -> &str {
        "Mutator_MustUseWithUnusedCall_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_function_names = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if !is_unit_type(return_type) {
                        if !has_must_use_attr(&func.attrs) {
                            func.attrs.insert(0, parse_quote!(#[must_use]));
                            modified_function_names.insert(func.sig.ident.clone());
                        }
                    }
                }
            }
        }

        let mut visitor = CallSiteVisitor {
            modified_functions: modified_function_names,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds the `#[must_use]` attribute to functions with non-void return types and modifies at least one call site to ignore the return value. This transformation introduces potential warnings for unused return values, testing the compiler's handling of `#[must_use]` attributes and their interaction with unused return values in function calls."
    }
}

fn is_unit_type(ty: &syn::Type) -> bool {
    if let syn::Type::Tuple(tuple) = ty {
        tuple.elems.is_empty()
    } else {
        false
    }
}

fn has_must_use_attr(attrs: &[syn::Attribute]) -> bool {
    attrs.iter().any(|attr| attr.path().is_ident("must_use"))
}

struct CallSiteVisitor {
    modified_functions: HashSet<syn::Ident>,
}

impl syn::visit_mut::VisitMut for CallSiteVisitor {
    fn visit_stmt_mut(&mut self, stmt: &mut syn::Stmt) {
        if let syn::Stmt::Local(local) = stmt {
            if let Some(syn::LocalInit { expr, .. }) = &mut local.init {
                if let syn::Expr::Call(call_expr) = &**expr {
                    if let syn::Expr::Path(path_expr) = &*call_expr.func {
                        if let Some(ident) = path_expr.path.get_ident() {
                            if self.modified_functions.contains(ident) {
                                *stmt = syn::Stmt::Expr(syn::Expr::Call(call_expr.clone()), None);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}