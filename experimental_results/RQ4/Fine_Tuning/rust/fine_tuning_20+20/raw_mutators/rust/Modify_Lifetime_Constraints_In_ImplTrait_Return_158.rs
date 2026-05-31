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

pub struct Modify_Lifetime_Constraints_In_ImplTrait_Return_158;

impl Mutator for Modify_Lifetime_Constraints_In_ImplTrait_Return_158 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Constraints_In_ImplTrait_Return_158"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_named_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Lifetime(_) = bound {
                                has_named_lifetime = true;
                                break;
                            }
                        }
                        if has_named_lifetime {
                            let new_bounds: Punctuated<TypeParamBound, Plus> = type_impl_trait
                                .bounds
                                .iter()
                                .map(|bound| match bound {
                                    TypeParamBound::Lifetime(_) => {
                                        TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site()))
                                    }
                                    _ => bound.clone(),
                                })
                                .collect();
                            let new_return_type = Type::ImplTrait(TypeImplTrait {
                                impl_token: type_impl_trait.impl_token,
                                bounds: new_bounds,
                            });
                            let new_output = ReturnType::Type(
                                item_fn.sig.output.span(),
                                Box::new(new_return_type),
                            );
                            item_fn.sig.output = new_output;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types that use named lifetimes. It modifies the return type's lifetime bounds to `'static`, creating a mismatch with the original lifetime constraints. This transformation aims to provoke lifetime-related compiler errors or ICEs by introducing non-universal lifetime regions in opaque type returns."
    }
}