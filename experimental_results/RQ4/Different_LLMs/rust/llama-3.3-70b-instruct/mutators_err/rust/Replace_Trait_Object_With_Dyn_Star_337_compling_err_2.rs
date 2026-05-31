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

pub struct Replace_Trait_Object_With_Dyn_Star_337;

impl Mutator for Replace_Trait_Object_With_Dyn_Star_337 {
    fn name(&self) -> &str {
        "Replace_Trait_Object_With_Dyn_Star_337"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(trait_object) = &*pat_type.ty {
                            let dyn_star_trait_object = Type::TraitObject(TypeTraitObject {
                                dyn_token: Some(token::Dyn::default()),
                                bounds: trait_object.bounds.clone(),
                            });
                            pat_type.ty = Box::new(dyn_star_trait_object);
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::TraitObject(trait_object) = &**return_type {
                        let dyn_star_trait_object = Type::TraitObject(TypeTraitObject {
                            dyn_token: Some(token::Dyn::default()),
                            bounds: trait_object.bounds.clone(),
                        });
                        *return_type = Box::new(dyn_star_trait_object);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(trait_object) = &*pat_type.ty {
                                    let dyn_star_trait_object = Type::TraitObject(TypeTraitObject {
                                        dyn_token: Some(token::Dyn::default()),
                                        bounds: trait_object.bounds.clone(),
                                    });
                                    pat_type.ty = Box::new(dyn_star_trait_object);
                                }
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::TraitObject(trait_object) = &**return_type {
                                let dyn_star_trait_object = Type::TraitObject(TypeTraitObject {
                                    dyn_token: Some(token::Dyn::default()),
                                    bounds: trait_object.bounds.clone(),
                                });
                                *return_type = Box::new(dyn_star_trait_object);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait objects with their dyn* counterparts, introducing the dyn* syntax to test the compiler's handling of trait object conversions and dyn* types."
    }
}