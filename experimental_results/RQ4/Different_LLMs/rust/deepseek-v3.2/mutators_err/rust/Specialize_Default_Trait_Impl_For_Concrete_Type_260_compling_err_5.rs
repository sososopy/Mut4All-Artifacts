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

pub struct Specialize_Default_Trait_Impl_For_Concrete_Type_260;

impl Mutator for Specialize_Default_Trait_Impl_For_Concrete_Type_260 {
    fn name(&self) -> &str {
        "Specialize_Default_Trait_Impl_For_Concrete_Type_260"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        let mut concrete_types = HashSet::new();
        let mut trait_with_default = None;
        let mut concrete_impls = Vec::new();
        
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                trait_names.insert(trait_item.ident.to_string());
            }
            if let Item::Impl(impl_item) = item {
                if let Type::Path(type_path) = &*impl_item.self_ty {
                    concrete_types.insert(type_path.path.segments.last().unwrap().ident.to_string());
                }
                for impl_item_inner in &impl_item.items {
                    if let ImplItem::Type(type_item) = impl_item_inner {
                        if type_item.ident == "Output" {
                            if let Type::Path(type_path) = &*impl_item.self_ty {
                                concrete_impls.push(type_path.path.segments.last().unwrap().ident.clone());
                            }
                        }
                    }
                }
            }
        }
        
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.items.iter().any(|item| matches!(item, TraitItem::Type(_))) {
                    continue;
                }
                trait_with_default = Some(trait_item.ident.clone());
                trait_item.items.push(parse_quote! {
                    type Output = bool;
                });
            }
        }
        
        if let Some(trait_name) = trait_with_default {
            for concrete_type in concrete_types {
                if concrete_impls.contains(&Ident::new(&concrete_type, Span::call_site())) {
                    let new_trait_name = if trait_names.contains("T2") {
                        Ident::new("T2", Span::call_site())
                    } else {
                        Ident::new("T2", Span::call_site())
                    };
                    
                    if !trait_names.contains("T2") {
                        file.items.push(parse_quote! {
                            trait T2 {}
                        });
                    }
                    
                    file.items.push(parse_quote! {
                        impl T2 for <#concrete_type as #trait_name>::Output {}
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator specializes a default trait implementation for a concrete type that appears in an associated type projection within another trait implementation. It first ensures the trait has an associated type Output (adding bool if missing). Then, for each concrete type implementing the2 trait, it adds a new trait T2 (or uses existing) and implements T2 for the associated type projection <ConcreteType as Trait>::Output. This creates overlapping specializations and normalized associated types, testing compiler specialization and coherence logic, potentially triggering assertion failures in intercrate mode."
    }
}