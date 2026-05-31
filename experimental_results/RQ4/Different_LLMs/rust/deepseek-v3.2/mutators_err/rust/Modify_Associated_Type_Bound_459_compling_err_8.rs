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

pub struct Modify_Associated_Type_Bound_459;

impl Mutator for Modify_Associated_Type_Bound_459 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Bound_459"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        let mut trait_definitions = Vec::new();
        
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                trait_names.insert(trait_item.ident.to_string());
                trait_definitions.push((trait_item.ident.clone(), trait_item));
            }
        }
        
        let mut new_trait_added = false;
        let mut traits_to_add = Vec::new();
        
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for associated_item in &mut trait_item.items {
                    if let syn::TraitItem::Type(assoc_type) = associated_item {
                        let bounds = &mut assoc_type.bounds;
                        if !bounds.is_empty() {
                            let mut found_trait_bound = false;
                            let mut original_bound = None;
                            
                            for bound in bounds.iter() {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    found_trait_bound = true;
                                    original_bound = Some(trait_bound.path.segments.last().unwrap().ident.to_string());
                                    break;
                                }
                            }
                            
                            if found_trait_bound {
                                let mut available_traits: Vec<String> = trait_names.iter()
                                    .filter(|&name| {
                                        if let Some(orig) = &original_bound {
                                            name != orig
                                        } else {
                                            true
                                        }
                                    })
                                    .cloned()
                                    .collect();
                                
                                if available_traits.is_empty() && !new_trait_added {
                                    let new_trait_name = Ident::new("Other", Span::call_site());
                                    let new_trait: Item = parse_quote! {
                                        trait #new_trait_name {}
                                    };
                                    
                                    traits_to_add.push(new_trait);
                                    trait_names.insert("Other".to_string());
                                    new_trait_added = true;
                                    available_traits.push("Other".to_string());
                                }
                                
                                if let Some(new_trait_name) = available_traits.choose(&mut thread_rng()) {
                                    bounds.clear();
                                    let new_path = SynPath::from(Ident::new(new_trait_name, Span::call_site()));
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: new_path,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for trait_to_add in traits_to_add {
            file.items.insert(0, trait_to_add);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with associated types that have trait bounds. It collects all trait names in scope and replaces the bound trait with a different existing trait. If no other trait exists, it creates a new trivial trait 'Other' and uses it as the bound. This creates projection type mismatches between the bound and actual associated type assignments in implementations, potentially triggering coherence checking errors and internal compiler errors in the next-solver coherence mode."
    }
}