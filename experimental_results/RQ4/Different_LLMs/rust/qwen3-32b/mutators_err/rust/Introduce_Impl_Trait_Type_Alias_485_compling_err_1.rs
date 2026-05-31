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

pub struct Introduce_Impl_Trait_Type_Alias_485;

impl Mutator for Introduce_Impl_Trait_Type_Alias_485 {
    fn name(&self) -> &str {
        "Introduce_Impl_Trait_Type_Alias_485"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all user-defined traits in the file
        let mut traits = vec![];
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                traits.push(trait_item.ident.clone());
            }
        }

        // Select a trait to use in the impl Trait
        let selected_trait_path = if !traits.is_empty() {
            // Randomly select a trait from the list
            let selected_trait = traits.choose(&mut thread_rng()).unwrap();
            // Create a path for the trait (assuming it's in the same module)
            let trait_path = SynPath {
                leading_colon: None,
                segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                    ident: selected_trait.clone(),
                    arguments: PathArguments::None,
                }]),
            };
            trait_path
        } else {
            // Use std::fmt::Debug as a fallback
            let debug_path = SynPath {
                leading_colon: Some(Default::default()),
                segments: {
                    let mut segments = punctuated::Punctuated::new();
                    segments.push(syn::PathSegment {
                        ident: Ident::new("fmt", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments.push(syn::PathSegment {
                        ident: Ident::new("Debug", Span::call_site()),
                        arguments: PathArguments::None,
                    });
                    segments
                },
            };
            debug_path
        };

        // Find the first applicable impl block and modify it
        for (i, item) in file.items.iter_mut().enumerate() {
            if let syn::Item::Impl(impl_item) = item {
                // Check if the self_ty is a concrete type (e.g., not a generic parameter)
                if let Type::Path(type_path) = &**impl_item.self_ty {
                    // Create the new type alias
                    let alias_name = Ident::new("Alias", Span::call_site());
                    let alias_type = Type::ImplTrait(TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: selected_trait_path.clone(),
                            }));
                            bounds
                        },
                    });

                    // Create the type alias item
                    let alias_item = Item::Type(syn::ItemType {
                        attrs: vec![],
                        ident: alias_name.clone(),
                        vis: syn::Visibility::Inherited,
                        colon_token: None,
                        ty: Box::new(alias_type),
                        where_clause: None,
                    });

                    // Insert the type alias before the impl block
                    file.items.insert(i, alias_item);

                    // Replace the self_ty of the impl block with the alias
                    *impl_item.self_ty = Box::new(Type::Path(syn::TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                ident: alias_name,
                                arguments: PathArguments::None,
                            }]),
                        },
                    }));

                    // Process only the first applicable impl block
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}