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

use std::collections::HashMap;

use crate::mutator::Mutator;

pub struct Replace_GAT_Parameter_With_Direct_Type_131;

impl Mutator for Replace_GAT_Parameter_With_Direct_Type_131 {
    fn name(&self) -> &str {
        "Replace_GAT_Parameter_With_Direct_Type_131"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let trait_ident = &item_trait.ident;
                let mut associated_types = Vec::new();
                for item in &item_trait.items {
                    if let TraitItem::Type(item_type) = item {
                        associated_types.push(item_type.ident.clone());
                    }
                }
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        let mut replacement_map = HashMap::new();
                        for (index, input) in item_fn.sig.inputs.iter().enumerate() {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                                    let mut new_type = None;
                                    for bound in &type_impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path.segments.len() == 1
                                                && associated_types.contains(
                                                    &trait_bound.path.segments[0].ident,
                                                )
                                            {
                                                let new_path: TypePath = parse_quote! {
                                                    <#trait_ident as #trait_ident>::#trait_bound
                                                };
                                                new_type = Some(Type::Path(new_path));
                                                break;
                                            }
                                        }
                                    }
                                    if let Some(new_type) = new_type {
                                        replacement_map.insert(index, new_type);
                                    }
                                }
                            }
                        }
                        for (index, new_type) in replacement_map {
                            if let FnArg::Typed(pat_type) = &mut item_fn.sig.inputs[index] {
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                let mut associated_types = HashMap::new();
                for item in &item_impl.items {
                    if let ImplItem::Type(item_type) = item {
                        associated_types.insert(item_type.ident.clone(), item_type.ty.clone());
                    }
                }
                for item in &mut item_impl.items {
                    if let ImplItem::Fn(item_fn) = item {
                        let mut replacement_map = HashMap::new();
                        for (index, input) in item_fn.sig.inputs.iter().enumerate() {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                                    let mut new_type = None;
                                    for bound in &type_impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if trait_bound.path.segments.len() == 1 {
                                                if let Some(assoc_type) =
                                                    associated_types.get(&trait_bound.path.segments[0].ident)
                                                {
                                                    new_type = Some(assoc_type.clone());
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if let Some(new_type) = new_type {
                                        replacement_map.insert(index, new_type);
                                    }
                                }
                            }
                        }
                        for (index, new_type) in replacement_map {
                            if let FnArg::Typed(pat_type) = &mut item_fn.sig.inputs[index] {
                                pat_type.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait methods that use generic associated types (GATs) in their parameter list, specifically those using `impl Trait` with a trait bound referencing a GAT. It replaces such parameters with a concrete type that directly references the associated type defined in the trait or its implementation. This transformation challenges the compiler's ability to resolve associated types and handle impl trait desugaring, potentially exposing weaknesses in trait resolution and type inference."
    }
}