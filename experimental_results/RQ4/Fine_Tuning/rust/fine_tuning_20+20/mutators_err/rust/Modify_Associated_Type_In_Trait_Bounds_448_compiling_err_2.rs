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

pub struct Modify_Associated_Type_In_Trait_Bounds_448;

impl Mutator for Modify_Associated_Type_In_Trait_Bounds_448 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_In_Trait_Bounds_448"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TraitCollector {
            trait_names: HashSet<String>,
        }

        impl<'ast> Visit<'ast> for TraitCollector {
            fn visit_item_trait(&mut self, i: &'ast ItemTrait) {
                self.trait_names.insert(i.ident.to_string());
            }
        }

        let mut trait_collector = TraitCollector {
            trait_names: HashSet::new(),
        };
        trait_collector.visit_file(file);

        struct TraitImplFinder {
            target_trait: String,
            found: bool,
        }

        impl<'ast> Visit<'ast> for TraitImplFinder {
            fn visit_item_impl(&mut self, i: &'ast ItemImpl) {
                if let Some((_, path, _)) = &i.trait_ {
                    if path.segments.last().unwrap().ident == self.target_trait {
                        self.found = true;
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(assoc_constraint) =
                                    trait_bound.path.segments.last().unwrap().arguments
                                        .as_angle_bracketed()
                                        .and_then(|args| {
                                            args.args.iter().find_map(|arg| {
                                                if let GenericArgument::Constraint(c) = arg {
                                                    Some(c)
                                                } else {
                                                    None
                                                }
                                            })
                                        })
                                {
                                    if assoc_constraint.ident == "Assoc" {
                                        let mut available_traits: Vec<String> =
                                            trait_collector.trait_names.iter().cloned().collect();
                                        available_traits.retain(|t| t != "Trait");
                                        let mut rng = thread_rng();
                                        available_traits.shuffle(&mut rng);
                                        for candidate in available_traits {
                                            let mut finder = TraitImplFinder {
                                                target_trait: candidate.clone(),
                                                found: false,
                                            };
                                            finder.visit_file(file);
                                            if !finder.found {
                                                assoc_constraint.ident =
                                                    Ident::new(&candidate, assoc_constraint.ident.span());
                                                break;
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
        "This mutator targets functions returning `impl Trait` with associated type constraints. It identifies such functions and attempts to replace the associated type constraint with a different trait that is not currently implemented in the code. By selecting a non-implemented trait, the mutator introduces a mismatch between the return type's expected associated type and the available trait implementations, likely leading to compilation errors or ICEs due to unresolved trait bounds."
    }
}