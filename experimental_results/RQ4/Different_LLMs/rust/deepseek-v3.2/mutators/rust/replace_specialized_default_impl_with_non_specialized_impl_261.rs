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

pub struct Replace_Specialized_Default_Impl_With_Non_Specialized_Impl_261;

impl Mutator for Replace_Specialized_Default_Impl_With_Non_Specialized_Impl_261 {
    fn name(&self) -> &str {
        "Replace_Specialized_Default_Impl_With_Non_Specialized_Impl_261"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut default_impls = Vec::new();
        let mut concrete_types = HashSet::new();

        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.defaultness.is_some() {
                    default_impls.push(item_impl.clone());
                }
            }
            if let Item::Struct(item_struct) = item {
                concrete_types.insert(item_struct.ident.to_string());
            }
            if let Item::Enum(item_enum) = item {
                concrete_types.insert(item_enum.ident.to_string());
            }
        }

        concrete_types.extend(vec!["i32", "u8", "bool", "String", "char", "f64", "usize", "isize"].into_iter().map(|s| s.to_string()));

        for default_impl in default_impls {
            let mut generic_params = Vec::new();
            for param in &default_impl.generics.params {
                if let GenericParam::Type(type_param) = param {
                    generic_params.push(type_param.ident.to_string());
                }
            }

            let mut candidate_types = Vec::new();
            if let Some((_, trait_path, _)) = &default_impl.trait_ {
                for bound in trait_path.segments.iter() {
                    let trait_name = bound.ident.to_string();
                    if trait_name == "Clone" || trait_name == "Debug" || trait_name == "Default" {
                        continue;
                    } else {
                        break;
                    }
                }
                for concrete_type in &concrete_types {
                    candidate_types.push(concrete_type.clone());
                }
            }

            if candidate_types.is_empty() {
                continue;
            }

            let chosen_type = candidate_types.choose(&mut thread_rng()).unwrap();

            let mut new_impl = default_impl.clone();
            new_impl.defaultness = None;

            for param in &mut new_impl.generics.params {
                if let GenericParam::Type(type_param) = param {
                    if generic_params.contains(&type_param.ident.to_string()) {
                        type_param.ident = Ident::new(chosen_type, Span::call_site());
                    }
                }
            }

            let mut visitor = TypeReplacementVisitor {
                generic_params: &generic_params,
                concrete_type: chosen_type,
            };
            syn::visit_mut::visit_item_impl_mut(&mut visitor, &mut new_impl);

            let mut existing_impls = HashSet::new();
            for item in &file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, trait_path, _)) = &item_impl.trait_ {
                        let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                        if let Some((_, default_trait_path, _)) = &default_impl.trait_ {
                            let default_trait_name = default_trait_path.segments.last().unwrap().ident.to_string();
                            if trait_name == default_trait_name {
                                if let Type::Path(type_path) = &*item_impl.self_ty {
                                    let type_name = type_path.path.segments.last().unwrap().ident.to_string();
                                    existing_impls.insert((trait_name,type_name));
                                }
                            }
                        }
                    }
                }
            }

            if let Some((_, default_trait_path, _)) = &default_impl.trait_ {
                let new_trait_name = default_trait_path.segments.last().unwrap().ident.to_string();
                if !existing_impls.contains(&(new_trait_name, chosen_type.clone())) {
                    file.items.push(Item::Impl(new_impl));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies default trait implementations and replaces them with non-default implementations for concrete types. It collects concrete types from the seed program and built-in types, selects a candidate type that satisfies the trait bounds (simplified by checking common bounds), and creates a specialized impl. This transformation tests the compiler's specialization graph construction and trait resolution logic, potentially triggering bugs similar to the reported ICE."
    }
}

struct TypeReplacementVisitor<'a> {
    generic_params: &'a Vec<String>,
    concrete_type: &'a str,
}

impl<'a> VisitMut for TypeReplacementVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if type_path.path.segments.len() == 1 {
                let ident = type_path.path.segments[0].ident.to_string();
                if self.generic_params.contains(&ident) {
                    type_path.path.segments[0].ident = Ident::new(self.concrete_type, Span::call_site());
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}