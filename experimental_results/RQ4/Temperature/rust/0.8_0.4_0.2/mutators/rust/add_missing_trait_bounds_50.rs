use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Add_Missing_Trait_Bounds_50;

impl Mutator for Add_Missing_Trait_Bounds_50 {
    fn name(&self) -> &str {
        "Add_Missing_Trait_Bounds_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                let mut missing_trait_bounds = Vec::new();

                // Check if the function uses traits that are not in the bounds
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = &**func {
                                if path.segments.last().unwrap().ident == "some_trait_method" {
                                    missing_trait_bounds.push("SomeTrait");
                                }
                            }
                        }
                    }
                }

                // Add missing trait bounds
                if !missing_trait_bounds.is_empty() {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            for trait_bound in &missing_trait_bounds {
                                type_param.bounds.push(parse_quote!(#trait_bound));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inspects function definitions for usage of traits that are not declared in the generic parameter bounds. If such traits are found, they are added to the bounds, ensuring the function's implementation aligns with its constraints. This helps uncover potential issues with trait satisfaction and improves the robustness of the type system."
    }
}