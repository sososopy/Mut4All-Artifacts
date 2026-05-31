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

pub struct Modify_Const_Fn_Return_Type_207;

impl Mutator for Modify_Const_Fn_Return_Type_207 {
    fn name(&self) -> &str {
        "Modify_Const_Fn_Return_Type_207"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let new_bound = TraitBound {
                                        paren_token: trait_bound.paren_token.clone(),
                                        modifier: trait_bound.modifier.clone(),
                                        lifetimes: trait_bound.lifetimes.clone(),
                                        path: {
                                            let mut new_path = trait_bound.path.clone();
                                            let last_segment = new_path.segments.last_mut();
                                            if let Some(segment) = last_segment {
                                                segment.ident = Ident::new(
                                                    &format!("~const {}", segment.ident),
                                                    segment.ident.span(),
                                                );
                                            }
                                            new_path
                                        },
                                    };
                                    new_bounds.push(TypeParamBound::Trait(new_bound));
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            type_impl_trait.bounds = new_bounds;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets `const fn` declarations with `impl Trait` return types. It modifies the return type by prefixing trait bounds with `~const`, mimicking a syntax error that can lead to compiler panics. This transformation stresses the compiler's type resolution and parsing logic for const functions with opaque return types."
    }
}