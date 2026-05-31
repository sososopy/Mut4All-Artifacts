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

pub struct Replace_ImplTrait_Return_With_Unit_2;

impl Mutator for Replace_ImplTrait_Return_With_Unit_2 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_Unit_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::ImplTrait(_) = **ty {
                        **ty = parse_quote!(()); // Replace impl Trait with unit type
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let syn::Type::ImplTrait(_) = **ty {
                                **ty = parse_quote!(()); // Replace impl Trait with unit type
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces impl Trait return types with the unit type (). This transformation can cause type mismatches, especially when the function body returns a non-unit value, leading to compilation errors or unexpected behavior. It targets functions and methods with impl Trait returns, potentially exposing bugs in type inference, trait resolution, or error reporting for impl Trait in return position."
    }
}

pub struct Add_Unsafe_Block_To_Expression_2;

impl Mutator for Add_Unsafe_Block_To_Expression_2 {
    fn name(&self) -> &str {
        "Add_Unsafe_Block_To_Expression_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AddUnsafeBlockVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator wraps arbitrary expressions in unsafe blocks. This can cause undefined behavior if the expression relies on safety invariants, and may trigger compiler warnings or errors about unsafe code in safe contexts. It targets a wide range of expression types, potentially exposing bugs in unsafe analysis, lint interactions, or safety checking."
    }
}

struct AddUnsafeBlockVisitor;

impl VisitMut for AddUnsafeBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Unsafe(_) = expr {
            // Do not wrap already unsafe blocks
        } else {
            let old_expr = std::mem::replace(expr, parse_quote!(()));
            *expr = parse_quote!(unsafe { #old_expr });
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

pub struct Replace_Binding_With_Wildcard_In_Match_202;

impl Mutator for Replace_Binding_With_Wildcard_In_Match_202 {
    fn name(&self) -> &str {
        "Replace_Binding_With_Wildcard_In_Match_202"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceBindingWithWildcardVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces bindings in match patterns with wildcards. This can cause unused variable warnings or errors if the binding is used in the match arm, and may affect exhaustiveness checking. It targets match patterns with bindings, potentially exposing bugs in pattern matching, binding analysis, or exhaustiveness checking."
    }
}

struct ReplaceBindingWithWildcardVisitor;

impl VisitMut for ReplaceBindingWithWildcardVisitor {
    fn visit_pat_mut(&mut self, pat: &mut syn::Pat) {
        if let syn::Pat::Ident(pat_ident) = pat {
            if pat_ident.by_ref.is_none() && pat_ident.mutability.is_none() {
                *pat = parse_quote!(_);
            }
        }
        syn::visit_mut::visit_pat_mut(self, pat);
    }
}