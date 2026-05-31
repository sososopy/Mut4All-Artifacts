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

pub struct Introduce_Higher_Ranked_Trait_Bound_30;

impl Mutator for Introduce_Higher_Ranked_Trait_Bound_30 {
    fn name(&self) -> &str {
        "Introduce_Higher_Ranked_Trait_Bound_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_candidates = Vec::new();
        let mut struct_candidates = Vec::new();
        let mut impl_candidates = Vec::new();
        let mut function_candidates = Vec::new();

        for item in &mut file.items {
            match item {
                Item::Trait(trait_item) => {
                    for trait_item_item in &trait_item.items {
                        if let syn::TraitItem::Type(type_item) = trait_item_item {
                            if let Some(generics) = &type_item.generics {
                                if generics.params.iter().any(|p| matches!(p, GenericParam::Lifetime(_))) {
                                    trait_candidates.push((trait_item.ident.clone(), type_item.ident.clone(), generics.clone()));
                                }
                            }
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    if let Some(generics) = &struct_item.generics {
                        for bound in generics.params.iter().filter_map(|p| match p {
                            GenericParam::Type(type_param) => Some(&type_param.bounds),
                            _ => None,
                        }).flatten() {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let trait_path = &trait_bound.path;
                                trait_candidates.iter().for_each(|(trait_name, _, _)| {
                                    if trait_path.segments.last().map(|s| &s.ident) == Some(&trait_name) {
                                        struct_candidates.push((struct_item.clone(), trait_name.clone()));
                                    }
                                });
                            }
                        }
                    }
                }
                Item::Impl(impl_item) => {
                    if let Some(trait_path) = &impl_item.trait_ {
                        trait_candidates.iter().for_each(|(trait_name, _, _)| {
                            if trait_path.1.segments.last().map(|s| &s.ident) == Some(&trait_name) {
                                impl_candidates.push(impl_item.clone());
                            }
                        });
                    }
                }
                Item::Fn(func_item) => {
                    for arg in &func_item.sig.inputs {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                struct_candidates.iter().for_each(|(struct_item, _)| {
                                    if type_path.path.segments.last().map(|s| &s.ident) == Some(&struct_item.ident) {
                                        function_candidates.push(func_item.clone());
                                    }
                                });
                            }
                        }
                    }
                }
                _ => {}
            }
        }

        for (struct_item, trait_name) in struct_candidates {
            let trait_info = trait_candidates.iter().find(|(name, _, _)| *name == trait_name);
            if let Some((_, assoc_type_name, assoc_generics)) = trait_info {
                let mut new_struct = struct_item.clone();
                if let Some(ref mut generics) = new_struct.generics {
                    let mut where_clause = generics.where_clause.clone().unwrap_or_else(|| {
                        WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        }
                    });
                    let lifetime_param = assoc_generics.params.iter().find_map(|p| match p {
                        GenericParam::Lifetime(lifetime_param) => Some(lifetime_param.lifetime.clone()),
                        _ => None,
                    });
                    if let Some(lifetime) = lifetime_param {
                        let higher_ranked_bound: WherePredicate = parse_quote! {
                            for<#lifetime> <Self as #trait_name>::#assoc_type_name<#lifetime>: Sized
                        };
                        where_clause.predicates.push(higher_ranked_bound);
                    }
                    generics.where_clause = Some(where_clause);
                }
                for item in &mut file.items {
                    if let Item::Struct(existing_struct) = item {
                        if existing_struct.ident == struct_item.ident {
                            *existing_struct = new_struct;
                            break;
                        }
                    }
                }
            }
        }

        for impl_item in impl_candidates {
            let trait_info = trait_candidates.iter().find(|(name, _, _)| *name == impl_item.trait_.as_ref().unwrap().1.segments.last().unwrap().ident);
            if let Some((_, assoc_type_name, _)) = trait_info {
                for impl_item_item in &mut impl_item.items {
                    if let syn::ImplItem::Type(type_item) = impl_item_item {
                        if type_item.ident == *assoc_type_name {
                            let mut new_generics = type_item.generics.clone().unwrap_or_else(|| {
                                Generics {
                                    params: Punctuated::new(),
                                    where_clause: None,
                                }
                            });
                            let new_lifetime_param: GenericParam = parse_quote! { 'a };
                            new_generics.params.push(new_lifetime_param);
                            let where_clause = new_generics.where_clause.clone().unwrap_or_else(|| {
                                WhereClause {
                                    where_token: Default::default(),
                                    predicates: Punctuated::new(),
                                }
                            });
                            let new_predicate: WherePredicate = parse_quote! { Self: 'a };
                            where_clause.predicates.push(new_predicate);
                            new_generics.where_clause = Some(where_clause);
                            type_item.generics = Some(new_generics);
                        }
                    }
                }
            }
            for item in &mut file.items {
                if let Item::Impl(existing_impl) = item {
                    if existing_impl.self_ty == impl_item.self_ty && existing_impl.trait_ == impl_item.trait_ {
                        *existing_impl = impl_item;
                        break;
                    }
                }
            }
        }

        for func_item in function_candidates {
            for arg in &mut func_item.sig.inputs {
                if let FnArg::Typed(pat_type) = arg {
                    if let Type::Path(type_path) = &*pat_type.ty {
                        let lifetime: Lifetime = parse_quote! { 'static };
                        let mut new_segments = type_path.path.segments.clone();
                        if let Some(last_segment) = new_segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                args.args.push(GenericArgument::Lifetime(lifetime));
                            } else {
                                last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: Default::default(),
                                    args: Punctuated::from_iter(vec![GenericArgument::Lifetime(lifetime)]),
                                    gt_token: Default::default(),
                                });
                            }
                        }
                        pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: new_segments,
                            },
                        }));
                    }
                }
            }
            for item in &mut file.items {
                if let Item::Fn(existing_func) = item {
                    if existing_func.sig.ident == func_item.sig.ident {
                        *existing_func = func_item;
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with lifetime-associated types, structs using those traits, and functions using those structs. It introduces higher-ranked trait bounds (for<'a>) on the associated type in the struct's where clause, modifies trait implementations to add a placeholder lifetime constraint (Self: 'a), and updates function calls to use a concrete lifetime ('static). This creates region conversion conflicts, testing the borrow checker's handling of higher-ranked trait bounds and lifetime mismatches."
    }
}