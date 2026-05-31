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

pub struct Modify_ImplTrait_Return_Type_14;

impl Mutator for Modify_ImplTrait_Return_Type_14 {
    fn name(&self) -> &str {
        "Modify_ImplTrait_Return_Type_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let original_bounds = &type_impl_trait.bounds;
                        let mut new_bounds = Punctuated::new();
                        for bound in original_bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let new_trait_ident = Ident::new("Debug", Span::call_site());
                                let new_trait_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: new_trait_ident,
                                                arguments: PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                };
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            }
                        }
                        if !new_bounds.is_empty() {
                            *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: token::Impl { span: Span::call_site() },
                                bounds: new_bounds,
                            }));
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