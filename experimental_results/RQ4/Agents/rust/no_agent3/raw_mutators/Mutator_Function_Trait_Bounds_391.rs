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

pub struct Mutator_Function_Trait_Bounds_391;

impl Mutator for Mutator_Function_Trait_Bounds_391 {
    fn name(&self) -> &str {
        "Mutator_Function_Trait_Bounds_391"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let mut has_trait_bounds = false;
                let mut new_bounds = Punctuated::new();

                // Check and modify the function signature for trait bounds
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(ref mut pat_type) = input {
                        if let Type::ImplTrait(ref mut impl_trait) = *pat_type.ty {
                            has_trait_bounds = true;
                            for bound in &impl_trait.bounds {
                                new_bounds.push(bound.clone());
                            }
                            new_bounds.push(parse_quote!(Send));
                            impl_trait.bounds = new_bounds.clone();
                        }
                    }
                }

                // Modify return type trait bounds if applicable
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::ImplTrait(ref mut impl_trait) = **ty {
                        for bound in &impl_trait.bounds {
                            new_bounds.push(bound.clone());
                        }
                        new_bounds.push(parse_quote!(Send));
                        impl_trait.bounds = new_bounds.clone();
                    }
                }

                // Modify the function body if trait bounds were found
                if has_trait_bounds {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(Expr::Cast(ref mut expr_cast)) = stmt {
                            let new_cast: Expr = parse_quote! {
                                {
                                    let casted: &dyn Send = &*expr_cast.expr;
                                    casted as _
                                }
                            };
                            *stmt = Stmt::Expr(new_cast);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with trait bounds, enhancing them by adding the Send trait. It modifies both input and output trait bounds and introduces a type casting operation to &dyn Send within the function body, aiming to expose potential compiler bugs related to trait bounds and type casting."
    }
}