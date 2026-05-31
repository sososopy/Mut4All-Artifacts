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

pub struct Replace_AssocConst_Bound_With_AssocType_294;

impl Mutator for Replace_AssocConst_Bound_With_AssocType_294 {
    fn name(&self) -> &str {
        "Replace_AssocConst_Bound_With_AssocType_294"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Check if trait T exists
        let trait_exists = file.items.iter().any(|item| {
            if let syn::Item::Trait(trait_item) = item {
                trait_item.ident == "T"
            } else {
                false
            }
        });

        if !trait_exists {
            let new_trait = parse_quote! {
                trait T {
                    type Assoc;
                }
            };
            file.items.insert(0, syn::Item::Trait(new_trait));
        }

        // Process all functions and methods
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                // Add generic parameter T: T
                let generic_param = parse_quote!(T: T);
                func.sig.generics.params.push(generic_param);

                // Process return type
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        self.process_impl_trait_bounds(type_impl_trait);
                    }
                }

                // Process function parameters
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let syn::Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            self.process_impl_trait_bounds(type_impl_trait);
                        }
                    }
                }
            }

            // Process impl items (methods)
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        // Add generic parameter T: T
                        let generic_param = parse_quote!(T: T);
                        method.sig.generics.params.push(generic_param);

                        // Process return type
                        if let syn::ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                self.process_impl_trait_bounds(type_impl_trait);
                            }
                        }

                        // Process method parameters
                        for arg in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let syn::Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                                    self.process_impl_trait_bounds(type_impl_trait);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces associated constant bounds in `impl Trait` type constraints with references to an associated type from a newly introduced trait `T`. It adds the trait `T` with an associated type `Assoc` and modifies the function or method to include a generic parameter `T: T`. This transformation forces the compiler to compare an associated constant against an opaque or projection type, potentially exposing assertion mismatches in type-checking logic."
    }
}

// Helper function to process the bounds of an impl Trait
impl Replace_AssocConst_Bound_With_AssocType_294 {
    fn process_impl_trait_bounds(&self, type_impl_trait: &mut TypeImplTrait) {
        for bound in &mut type_impl_trait.bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                let mut new_path = trait_bound.path.clone();
                for segment in new_path.segments.iter_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::AssocType(assoc_type) = arg {
                                // Replace the type with T::Assoc
                                let new_type = parse_quote!(T::Assoc);
                                *assoc_type.ty = new_type;
                            }
                        }
                    }
                }
                // Update the trait bound's path
                trait_bound.path = new_path;
            }
        }
    }
}