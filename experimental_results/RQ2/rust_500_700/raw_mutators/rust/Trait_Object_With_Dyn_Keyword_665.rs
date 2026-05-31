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

pub struct Trait_Object_With_Dyn_Keyword_665;

impl Mutator for Trait_Object_With_Dyn_Keyword_665 {
    fn name(&self) -> &str {
        "Trait_Object_With_Dyn_Keyword_665"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.qself.is_none() && type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                if segment.arguments.is_empty() {
                                    let trait_object = Type::TraitObject(TypeTraitObject {
                                        dyn_token: Some(token::Dyn::default()),
                                        bounds: {
                                            let mut bounds = Punctuated::new();
                                            bounds.push(TypeParamBound::Trait(TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: type_path.path.clone(),
                                            }));
                                            bounds
                                        },
                                    });
                                    pat_type.ty = Box::new(trait_object);
                                }
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if type_path.qself.is_none() && type_path.path.segments.len() == 1 {
                            let segment = &type_path.path.segments[0];
                            if segment.arguments.is_empty() {
                                let trait_object = Type::TraitObject(TypeTraitObject {
                                    dyn_token: Some(token::Dyn::default()),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: type_path.path.clone(),
                                        }));
                                        bounds
                                    },
                                });
                                *return_type = Box::new(trait_object);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}