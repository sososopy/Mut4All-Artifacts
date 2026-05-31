use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemType, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Mutate_Type_Alias_Impl_Trait_To_Concrete_Type_480;

impl Mutator for Mutate_Type_Alias_Impl_Trait_To_Concrete_Type_480 {
    fn name(&self) -> &str {
        "Mutate_Type_Alias_Impl_Trait_To_Concrete_Type_480"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidate_types: Vec<syn::Type> = Vec::new();
        let mut trait_names: HashSet<String> = HashSet::new();
        let mut type_aliases_to_mutate: Vec<(usize, syn::TypeParamBound)> = Vec::new();

        for (item_idx, item) in file.items.iter().enumerate() {
            if let Item::Type(alias) = item {
                if let Type::ImplTrait(impl_trait) = &*alias.ty {
                    for bound in &impl_trait.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                            trait_names.insert(trait_name);
                            type_aliases_to_mutate.push((item_idx, bound.clone()));
                        }
                    }
                }
            }
            if let Item::Struct(item_struct) = item {
                candidate_types.push(syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: syn::Path::from(item_struct.ident.clone()),
                }));
            }
            if let Item::Enum(item_enum) = item {
                candidate_types.push(syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: syn::Path::from(item_enum.ident.clone()),
                }));
            }
        }

        let mut new_items_to_insert = Vec::new();
        for (item_idx, bound) in type_aliases_to_mutate.iter().rev() {
            if let Item::Type(alias) = &mut file.items[*item_idx] {
                if let Type::ImplTrait(impl_trait) = &*alias.ty {
                    let mut found_concrete_type = None;
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                        for candidate in &candidate_types {
                            if let Type::Path(type_path) = candidate {
                                let type_name = type_path.path.segments.last().unwrap().ident.to_string();
                                if trait_names.contains(&type_name) {
                                    found_concrete_type = Some(candidate.clone());
                                    break;
                                }
                            }
                        }
                        if found_concrete_type.is_none() {
                            let new_struct_name = format!("{}Concrete", alias.ident);
                            let new_struct_ident = Ident::new(&new_struct_name, Span::call_site());
                            let new_struct_item: Item = parse_quote! {
                                struct #new_struct_ident {}
                            };
                            new_items_to_insert.push((0, new_struct_item));
                            let impl_item: Item = parse_quote! {
                                impl #trait_bound for #new_struct_ident {}
                            };
                            new_items_to_insert.push((1, impl_item));
                            found_concrete_type = Some(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(new_struct_ident),
                            }));
                        }
                        if let Some(concrete_type) = found_concrete_type {
                            *alias.ty = Box::new(concrete_type);
                        }
                    }
                }
            }
        }
        for (position, item) in new_items_to_insert.into_iter().rev() {
            file.items.insert(position, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type aliases defined with `impl Trait`. It scans the seed program for existing types that could satisfy the trait bound. If found, it replaces the `impl Trait` with that concrete type. Otherwise, it introduces a new struct with a trivial trait implementation and uses that struct as the concrete type. This transformation stresses the compiler's handling of type alias impl trait monomorphization, drop glue, and projection logic, potentially triggering bugs in MIR generation and trait resolution."
    }
}