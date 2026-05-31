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

pub struct Modify_Const_Impl_Trait_Bound_324;

impl Mutator for Modify_Const_Impl_Trait_Bound_324 {
    fn name(&self) -> &str {
        "Modify_Const_Impl_Trait_Bound_324"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some(trait_ref) = &impl_item.trait_ {
                    let trait_path = &trait_ref.1;
                    
                    if let Some(generics) = impl_item.generics.as_mut() {
                        for param in &mut generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                for bound in &mut type_param.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path == *trait_path {
                                            trait_bound.modifier = TraitBoundModifier::Forced;
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(where_clause) = &mut generics.where_clause {
                            for pred in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(pred_type) = pred {
                                    for bound in &mut pred_type.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path == *trait_path {
                                                trait_bound.modifier = TraitBoundModifier::Forced;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            let method_generics = &mut method.sig.generics;
                            for param in &mut method_generics.params {
                                if let syn::GenericParam::Type(type_param) = param {
                                    for bound in &mut type_param.bounds {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path == *trait_path {
                                                trait_bound.modifier = TraitBoundModifier::Forced;
                                            }
                                        }
                                    }
                                }
                            }
                            if let Some(where_clause) = &mut method_generics.where_clause {
                                for pred in &mut where_clause.predicates {
                                    if let syn::WherePredicate::Type(pred_type) = pred {
                                        for bound in &mut pred_type.bounds {
                                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path == *trait_path {
                                                    trait_bound.modifier = TraitBoundModifier::Forced;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            
                            for arg in &mut method.sig.inputs {
                                if let syn::FnArg::Typed(pat_type) = arg {
                                    if let syn::Type::TraitObject(trait_obj) = &mut *pat_type.ty {
                                        for bound in &mut trait_obj.bounds {
                                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path == *trait_path {
                                                    trait_bound.modifier = TraitBoundModifier::Forced;
                                                }
                                            }
                                        }
                                    }
                                }
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