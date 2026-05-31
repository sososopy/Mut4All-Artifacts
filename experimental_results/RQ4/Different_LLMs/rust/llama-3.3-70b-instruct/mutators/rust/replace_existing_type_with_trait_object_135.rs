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

pub struct Replace_Existing_Type_With_Trait_Object_135;

impl Mutator for Replace_Existing_Type_With_Trait_Object_135 {
    fn name(&self) -> &str {
        "Replace_Existing_Type_With_Trait_Object_135"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                let trait_bound = TypeTraitObject {
                                    dyn_token: Some(token::Dyn::default()),
                                    bounds: punctuated::Punctuated::from_iter(vec![TypeParamBound::Trait(
                                        TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: type_path.path.clone(),
                                        },
                                    )]),
                                };
                                *ty = Box::new(Type::TraitObject(trait_bound));
                            }
                        }
                    }
                }
            }
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &**ty {
                        let trait_bound = TypeTraitObject {
                            dyn_token: Some(token::Dyn::default()),
                            bounds: punctuated::Punctuated::from_iter(vec![TypeParamBound::Trait(
                                TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: type_path.path.clone(),
                                },
                            )]),
                        };
                        *ty = Box::new(Type::TraitObject(trait_bound));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing types with trait objects in function and method return types. It introduces dynamic dispatch and type erasure, testing the compiler's handling of trait objects and their interactions with generic types and trait bounds."
    }
}