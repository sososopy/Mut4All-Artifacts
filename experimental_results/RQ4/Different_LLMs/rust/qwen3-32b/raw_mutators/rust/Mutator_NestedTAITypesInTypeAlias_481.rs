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

pub struct Mutator_NestedTAITypesInTypeAlias_481;

impl Mutator for Mutator_NestedTAITypesInTypeAlias_481 {
    fn name(&self) -> &str {
        "Mutator_NestedTAITypesInTypeAlias_481"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all existing type names in the file
        let mut existing_types: Vec<Ident> = Vec::new();
        for item in &file.items {
            if let syn::Item::Struct(s) = item {
                existing_types.push(s.ident.clone());
            } else if let syn::Item::Enum(e) = item {
                existing_types.push(e.ident.clone());
            } else if let syn::Item::Union(u) = item {
                existing_types.push(u.ident.clone());
            } else if let syn::Item::Type(t) = item {
                existing_types.push(t.ident.clone());
            }
        }

        // If no existing types, nothing to do
        if existing_types.is_empty() {
            return;
        }

        // Process each item in the file
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(type_impl_trait) = &mut item_type.ty {
                    // Check if the bounds are a single TraitBound
                    if let Some(first_bound) = type_impl_trait.bounds.first() {
                        if let syn::TypeParamBound::Trait(trait_bound) = first_bound {
                            // Pick a random existing type
                            let mut rng = thread_rng();
                            if let Some(existing_type) = existing_types.choose(&mut rng) {
                                // Create the inner TAIT for the same trait
                                let inner_trait_path = trait_bound.path.clone();
                                let inner_impl_trait = TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            path: inner_trait_path.clone(),
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                        }));
                                        bounds
                                    },
                                };

                                // Create the existing type path
                                let existing_type_path = TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: existing_type.clone(),
                                                arguments: PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                };

                                // Create the tuple type (ExistingType, impl Iterator)
                                let tuple_type = TypeTuple {
                                    paren_token: Paren::default(),
                                    elems: {
                                        let mut elems = Punctuated::new();
                                        elems.push(Box::new(Type::Path(existing_type_path)));
                                        elems.push(Box::new(Type::ImplTrait(inner_impl_trait)));
                                        elems
                                    },
                                };

                                // Modify the trait bound to include the generic argument
                                let mut modified_path = trait_bound.path.clone();
                                let last_segment = modified_path.segments.last_mut().unwrap();
                                last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(GenericArgument::AssocType(AssocTypeArg {
                                            ident: Ident::new("Item", Span::call_site()),
                                            eq_token: Some(token::Eq::default()),
                                            ty: Box::new(tuple_type),
                                        }));
                                        args
                                    },
                                    gt_token: token::Gt::default(),
                                });

                                // Update the trait bound's path
                                trait_bound.path = modified_path;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms type aliases using `impl Trait` into nested opaque types by introducing an associated type parameter with a tuple containing an existing type and another `impl Trait`. This increases type complexity, stressing the compiler's handling of nested opaque types and potentially exposing ICEs related to trait resolution and type inference."
    }
}