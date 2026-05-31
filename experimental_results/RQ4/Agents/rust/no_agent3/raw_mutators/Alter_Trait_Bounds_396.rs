use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Trait_Bounds_396;

impl Mutator for Alter_Trait_Bounds_396 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TypeCastVisitor<'a> {
            rng: &'a mut dyn Rng,
        }

        impl<'a> VisitMut for TypeCastVisitor<'a> {
            fn visit_type_mut(&mut self, ty: &mut Type) {
                if let Type::TraitObject(trait_obj) = ty {
                    if let Some(first_bound) = trait_obj.bounds.first() {
                        if let syn::TypeParamBound::Trait(trait_bound) = first_bound {
                            let mut new_bounds = trait_obj.bounds.clone();

                            if self.rng.gen_bool(0.5) {
                                // Add a new trait bound
                                new_bounds.push(parse_quote!(Clone));
                            } else {
                                // Remove or replace an existing bound
                                if new_bounds.len() > 1 {
                                    if self.rng.gen_bool(0.5) {
                                        // Remove a bound
                                        new_bounds.pop();
                                    } else {
                                        // Replace a bound
                                        new_bounds.pop();
                                        new_bounds.push(parse_quote!(Sync));
                                    }
                                }
                            }

                            trait_obj.bounds = new_bounds;
                        }
                    }
                }

                syn::visit_mut::visit_type_mut(self, ty);
            }
        }

        let mut rng = thread_rng();
        let mut visitor = TypeCastVisitor { rng: &mut rng };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds in type casting expressions, specifically involving `dyn* Trait`. It modifies these bounds by adding, removing, or replacing them with other traits like `Clone` or `Sync`. This approach aims to test the robustness of the type checking mechanism, especially with the `dyn*` feature."
    }
}