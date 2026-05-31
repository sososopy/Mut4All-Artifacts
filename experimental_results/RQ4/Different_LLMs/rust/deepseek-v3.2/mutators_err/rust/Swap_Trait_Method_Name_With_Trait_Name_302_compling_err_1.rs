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

pub struct Swap_Trait_Method_Name_With_Trait_Name_302;

impl Mutator for Swap_Trait_Method_Name_With_Trait_Name_302 {
    fn name(&self) -> &str {
        "Swap_Trait_Method_Name_With_Trait_Name_302"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods = std::collections::HashMap::new();
        let mut trait_names = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                trait_names.push(trait_item.ident.clone());
                let method_names = trait_item.items.iter()
                    .filter_map(|item| match item {
                        syn::TraitItem::Fn(method) => Some(method.sig.ident.clone()),
                        _ => None,
                    })
                    .collect::<Vec<_>>();
                trait_methods.insert(trait_item.ident.clone(), method_names);
            }
        }
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let syn::Type::Path(type_path) = &*impl_item.trait_.as_ref().unwrap().1 {
                    let trait_name = &type_path.path.segments.last().unwrap().ident;
                    if trait_names.contains(trait_name) {
                        let methods = trait_methods.get(trait_name).cloned().unwrap_or_default();
                        for impl_item in &mut impl_item.items {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                if method.sig.ident == *trait_name {
                                    let mut rng = thread_rng();
                                    if methods.len() > 1 {
                                        let other_method = methods.choose(&mut rng).unwrap();
                                        method.sig.ident = other_method.clone();
                                        for other_impl_item in &mut impl_item.items {
                                            if let syn::ImplItem::Fn(other_method_item) = other_impl_item {
                                                if other_method_item.sig.ident == *other_method {
                                                    let new_name = Ident::new(format!("{}_mutated", trait_name), Span::call_site());
                                                    other_method_item.sig.ident = new_name;
                                                }
                                            }
                                        }
                                    } else {
                                        let new_name = Ident::new(format!("{}_mutated", trait_name), Span::call_site());
                                        method.sig.ident = new_name.clone();
                                        for trait_item in &mut file.items {
                                            if let syn::Item::Trait(trait_def) = trait_item {
                                                if trait_def.ident == *trait_name {
                                                    let new_trait_method: syn::TraitItem = parse_quote! {
                                                        fn #new_name(&self) -> i32;
                                                    };
                                                    trait_def.items.push(new_trait_method);
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
        "The mutation operator targets trait implementations where a method name matches the trait name. It swaps the name of such a method with another method name from the same trait, or introduces a new mutated method name if no other method exists. This transformation creates a mismatch between the implemented method name and the expected trait method name, potentially triggering internal compiler confusion about defaultness and associated items, and testing trait coherence updates."
    }
}