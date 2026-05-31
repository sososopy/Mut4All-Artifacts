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

pub struct Modify_ImplTrait_Return_To_Concrete_269;

fn extract_impl_trait_type(ty: &Type) -> Option<&TypeImplTrait> {
    if let Type::ImplTrait(type_impl_trait) = ty {
        Some(type_impl_trait)
    } else {
        None
    }
}

fn is_recursive_impl_trait(item_fn: &ItemFn) -> bool {
    if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
        if let Type::ImplTrait(type_impl_trait) = &**return_type {
            for bound in &type_impl_trait.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    for segment in &trait_bound.path.segments {
                        if segment.ident == "Handler" {
                            return true;
                        }
                    }
                }
            }
        }
    }
    false
}

fn replace_impl_trait_with_concrete_type(item_fn: &mut ItemFn) {
    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
        if let Type::ImplTrait(_) = &**return_type {
            *return_type = Box::new(parse_quote! { (fn() -> impl Handler,) });
        }
    }
}

fn replace_return_with_concrete_expr(item_fn: &mut ItemFn) {
    let concrete_expr: Expr = parse_quote! { (one,) };
    let mut new_block = item_fn.block.clone();
    new_block.stmts.clear();
    new_block.stmts.push(Stmt::Expr(concrete_expr, None));
    item_fn.block = new_block;
}

impl Mutator for Modify_ImplTrait_Return_To_Concrete_269 {
    fn name(&self) -> &str {
        "Modify_ImplTrait_Return_To_Concrete_269"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if is_recursive_impl_trait(item_fn) {
                    replace_impl_trait_with_concrete_type(item_fn);
                    replace_return_with_concrete_expr(item_fn);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types that are recursive, such as those returning a tuple with the function itself. It replaces the `impl Trait` with a concrete type that explicitly captures the intended structure, like `(fn() -> impl Handler,)`, and adjusts the return expression accordingly. This transformation helps explore how the compiler handles recursive type definitions and trait implementations without relying on `impl Trait`, potentially revealing issues in type resolution and trait system robustness."
    }
}