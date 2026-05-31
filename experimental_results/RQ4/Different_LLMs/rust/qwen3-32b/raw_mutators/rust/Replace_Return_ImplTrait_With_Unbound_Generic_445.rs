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

pub struct Replace_Return_ImplTrait_With_Unbound_Generic_445;

impl Mutator for Replace_Return_ImplTrait_With_Unbound_Generic_445 {
    fn name(&self) -> &str {
        "Replace_Return_ImplTrait_With_Unbound_Generic_445"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all type parameters from the AST
        let mut all_type_params: Vec<Ident> = Vec::new();
        for item in &file.items {
            match item {
                Item::Fn(func) => {
                    for param in &func.sig.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            all_type_params.push(type_param.ident.clone());
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    for param in &struct_item.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            all_type_params.push(type_param.ident.clone());
                        }
                    }
                }
                Item::Impl(impl_item) => {
                    for param in &impl_item.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            all_type_params.push(type_param.ident.clone());
                        }
                    }
                }
                _ => {}
            }
        }

        // Process each function
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                // Skip main function
                if func.sig.ident == "main" {
                    continue;
                }

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        // Check if there are any trait bounds
                        if !type_impl_trait.bounds.is_empty() {
                            // Collect the function's own type parameters
                            let mut func_type_params: HashSet<Ident> = HashSet::new();
                            for param in &func.sig.generics.params {
                                if let GenericParam::Type(type_param) = param {
                                    func_type_params.insert(type_param.ident.clone());
                                }
                            }

                            // Find an unrelated type parameter
                            if let Some(unrelated_param) = all_type_params.iter().find(|ident| !func_type_params.contains(ident)) {
                                // Modify the first trait bound
                                for bound in &mut type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        // Modify the path to include the generic argument
                                        let path = &mut trait_bound.path;
                                        let mut segments = path.segments.iter_mut().collect::<Vec<_>>();
                                        if let Some(segment) = segments.last_mut() {
                                            // Create the generic arguments
                                            let generic_args: PathArguments = parse_quote!(<#unrelated_param>);
                                            segment.arguments = generic_args;
                                        }
                                        break; // Only modify the first one
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
        ""
    }
}