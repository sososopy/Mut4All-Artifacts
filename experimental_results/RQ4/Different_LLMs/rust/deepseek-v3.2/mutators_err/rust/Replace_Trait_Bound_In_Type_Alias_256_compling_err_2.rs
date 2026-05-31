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

pub struct Replace_Trait_Bound_In_Type_Alias_256;

impl Mutator for Replace_Trait_Bound_In_Type_Alias_256 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_In_Type_Alias_256"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::Path(type_path) = &*type_alias.ty {
                    let mut modified = false;
                    let mut new_bounds = Punctuated::new();
                    for bound in &type_alias.generics.params {
                        if let GenericParam::Type(type_param) = bound {
                            for bound in &type_param.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if !modified {
                                        let mut path = trait_bound.path.clone();
                                        if path.segments.len() > 1 {
                                            path.segments.pop();
                                        } else {
                                            path.segments = Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("_", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]);
                                        }
                                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: trait_bound.paren_token,
                                            modifier: trait_bound.modifier,
                                            lifetimes: trait_bound.lifetimes,
                                            path,
                                        }));
                                        modified = true;
                                    } else {
                                        new_bounds.push(TypeParamBound::Trait(trait_bound.clone()));
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                        }
                    }
                    if modified {
                        for param in &mut type_alias.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                type_param.bounds = new_bounds.clone();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type alias definitions containing trait bounds. It modifies the first trait bound in the alias by either truncating its path (removing the last segment for multi-segment paths) or replacing it with an underscore placeholder for single-segment paths. This creates syntactically valid but semantically incomplete trait bounds, testing the compiler's diagnostic and resolution logic for malformed trait bounds in type aliases."
    }
}