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

pub struct Specialized_Impl_Overlap_With_Multiple_Associated_Type_Usages_455;

impl Mutator for Specialized_Impl_Overlap_With_Multiple_Associated_Type_Usages_455 {
    fn name(&self) -> &str {
        "Specialized_Impl_Overlap_With_Multiple_Associated_Type_Usages_455"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_trait_with_assoc = false;
        let mut existing_traits = Vec::new();
        let mut existing_types = HashSet::new();
        let mut existing_bounds = Vec::new();

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                existing_traits.push(trait_item.ident.clone());
                for trait_item in &trait_item.items {
                    if let syn::TraitItem::Type(_) = trait_item {
                        has_trait_with_assoc = true;
                        break;
                    }
                }
            }
            if let Item::Struct(struct_item) = item {
                existing_types.insert(struct_item.ident.clone());
            }
            if let Item::Type(type_item) = item {
                existing_types.insert(type_item.ident.clone());
            }
            if let Item::Enum(enum_item) = item {
                existing_types.insert(enum_item.ident.clone());
            }
        }

        let mut rng = thread_rng();
        let mut new_items = Vec::new();

        if !has_trait_with_assoc {
            let trait_name = if existing_traits.is_empty() {
                Ident::new("TraitA", Span::call_site())
            } else {
                existing_traits[rng.gen_range(0..existing_traits.len())].clone()
            };
            let new_trait: Item = parse_quote! {
                trait #trait_name {
                    type Output;
                }
            };
            new_items.push(new_trait);
            existing_traits.push(trait_name);
        }

        let trait_a = if existing_traits.is_empty() {
            Ident::new("TraitA", Span::call_site())
        } else {
            existing_traits[rng.gen_range(0..existing_traits.len())].clone()
        };

        let bound_trait = Ident::new("Clone", Span::call_site());
        existing_bounds.push(bound_trait);

        let concrete_type = if existing_types.is_empty() {
            Ident::new("u8", Span::call_site())
        } else {
            let types_vec: Vec<_> = existing_types.iter().collect();
            types_vec[rng.gen_range(0..types_vec.len())].clone()
        };

        let default_impl: Item = parse_quote! {
            default impl<T: #bound_trait> #trait_a for T {
                type Output = bool;
            }
        };
        new_items.push(default_impl);

        let concrete_impl: Item = parse_quote! {
            impl #trait_a for #concrete_type {
                type Output = bool;
            }
        };
        new_items.push(concrete_impl);

        let trait_b = Ident::new("TraitB", Span::call_site());
        let new_trait_b: Item = parse_quote! {
            trait #trait_b {}
        };
        new_items.push(new_trait_b);

        let second_concrete_type = if existing_types.len() < 2 {
            Ident::new("u16", Span::call_site())
        } else {
            let mut types_vec: Vec<_> = existing_types.iter().collect();
            types_vec.shuffle(&mut rng);
            let first_type = types_vec[0].clone();
            let second_type = types_vec[1 % types_vec.len()].clone();
            if second_type == concrete_type {
                types_vec[2 % types_vec.len()].clone()
            } else {
                second_type
            }
        };

        let impl1: Item = parse_quote! {
            impl #trait_b for <#concrete_type as #trait_a>::Output {}
        };
        new_items.push(impl1);

        let impl2: Item = parse_quote! {
            impl #trait_b for <#second_concrete_type as #trait_a>::Output {}
        };
        new_items.push(impl2);

        for new_item in new_items {
            file.items.push(new_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces the specialization bug pattern by ensuring a trait with an associated type exists, adding a default impl with a generic bound and a concrete impl that overlaps, then creating a second trait with multiple impls referencing the associated type from different concrete types. This replicates the structure that triggers ICEs in the compiler's specialization graph handling during coherence checking."
    }
}