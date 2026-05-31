use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, seq::IteratorRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Create_Circular_Assoc_Type_Dependency_261;

impl Mutator for Create_Circular_Assoc_Type_Dependency_261 {
    fn name(&self) -> &str {
        "Create_Circular_Assoc_Type_Dependency_261"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut assoc_type_nodes = Vec::new();

        // Collect all associated type nodes in trait impls
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_bound) = &impl_item.trait_ {
                    // Fixed: Access the Path via tuple index instead of .path field
                    let trait_path = &trait_bound.1;
                    let self_type = &impl_item.self_ty;
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Type(type_item) = impl_item {
                            assoc_type_nodes.push((trait_path.clone(), self_type.clone(), type_item.ident.clone(), type_item));
                        }
                    }
                }
            }
        }

        // If there are at least two associated types, modify two of them to create a cycle
        if assoc_type_nodes.len() >= 2 {
            let mut rng = thread_rng();
            let mut selected: Vec<_> = assoc_type_nodes.as_slice().choose_multiple(&mut rng, 2).collect();
            let (trait_path1, self_type1, assoc_name1, type_item1) = &mut selected[0];
            let (trait_path2, self_type2, assoc_name2, type_item2) = &mut selected[1];

            // Modify type_item1 to reference type_item2's associated type
            let new_type1 = parse_quote!(<#self_type2 as #trait_path2>::#assoc_name2);
            type_item1.ty = new_type1;

            // Modify type_item2 to reference type_item1's associated type
            let new_type2 = parse_quote!(<#self_type1 as #trait_path1>::#assoc_name1);
            type_item2.ty = new_type2;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces circular associated type dependencies between traits by randomly selecting two associated types from different trait implementations and modifying each to reference the other. This creates a cycle in the type system, potentially triggering substitution errors and testing the compiler's ability to resolve cyclic trait bounds and associated type references."
    }
}