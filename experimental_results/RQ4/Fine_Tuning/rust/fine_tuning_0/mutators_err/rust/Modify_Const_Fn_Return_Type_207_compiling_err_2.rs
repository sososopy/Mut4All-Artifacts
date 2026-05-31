use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath,
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
                        if let Type::ImplTrait(type_impl_trait) = &**return_type {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let mut new_path = trait_bound.path.clone();
                                    if let Some(segment) = new_path.segments.last_mut() {
                                        let new_ident = format!("~const_{}", segment.ident);
                                        segment.ident = Ident::new(&new_ident, segment.ident.span());
                                    }
                                    new_bounds.push(TypeParamBound::Trait(TraitBound {
                                        path: new_path,
                                        ..trait_bound.clone()
                                    }));
                                }
                            }
                            *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: type_impl_trait.impl_token,
                                bounds: new_bounds,
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `const fn` declarations with `impl Trait` return types. It modifies the trait path in the return type to include a non-existent keyword `~const`, which introduces a syntax error. This transformation aims to test the compiler's handling of erroneous return type syntax in `const fn` and provoke potential parsing or name resolution issues."
    }
}