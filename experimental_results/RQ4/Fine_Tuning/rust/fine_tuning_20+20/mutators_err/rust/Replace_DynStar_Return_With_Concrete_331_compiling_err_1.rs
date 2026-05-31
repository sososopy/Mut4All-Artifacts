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

pub struct Replace_DynStar_Return_With_Concrete_331;

impl Mutator for Replace_DynStar_Return_With_Concrete_331 {
    fn name(&self) -> &str {
        "Replace_DynStar_Return_With_Concrete_331"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::TraitObject(type_trait_object) = &**ty {
                        if type_trait_object
                            .bounds
                            .iter()
                            .any(|b| matches!(b, syn::TypeParamBound::Trait(tb) if tb.dyn_token.is_some()))
                        {
                            let mut found = false;
                            for stmt in &mut func.block.stmts {
                                if let syn::Stmt::Expr(expr, _) = stmt {
                                    if let syn::Expr::Cast(expr_cast) = expr {
                                        if let syn::Type::TraitObject(type_trait_object) =
                                            &*expr_cast.ty
                                        {
                                            if type_trait_object
                                                .bounds
                                                .iter()
                                                .any(|b| matches!(b, syn::TypeParamBound::Trait(tb) if tb.dyn_token.is_some()))
                                            {
                                                *ty = Box::new(*expr_cast.ty.clone());
                                                found = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            if found {
                                func.block.stmts.retain(|stmt| {
                                    if let syn::Stmt::Expr(expr, _) = stmt {
                                        if let syn::Expr::Cast(expr_cast) = expr {
                                            if let syn::Type::TraitObject(type_trait_object) =
                                                &*expr_cast.ty
                                            {
                                                if type_trait_object
                                                    .bounds
                                                    .iter()
                                                    .any(|b| matches!(b, syn::TypeParamBound::Trait(tb) if tb.dyn_token.is_some()))
                                                {
                                                    return false;
                                                }
                                            }
                                        }
                                    }
                                    true
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning `dyn* Trait` by replacing the return type with a concrete type used in the function's cast operations, specifically those casting to `dyn* Trait`. It removes or replaces these cast operations in the function body, ensuring compatibility with the new return type. This transformation leverages the new `dyn*` feature to stress-test the compiler's handling of dynamically-sized trait objects and their interactions with concrete types."
    }
}