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

pub struct Replace_Nested_ImplTrait_With_Generic_Param_446;

impl Mutator for Replace_Nested_ImplTrait_With_Generic_Param_446 {
    fn name(&self) -> &str {
        "Replace_Nested_ImplTrait_With_Generic_Param_446"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                trait_names.insert(trait_item.ident.to_string());
            }
        }
        if trait_names.is_empty() {
            return;
        }
        let mut visitor = ReplaceNestedImplTraitVisitor {
            trait_names,
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets nested impl Trait patterns within for<..> binders. It identifies return types or associated types matching `impl for<..> Trait<impl Trait<..>>` and replaces the inner impl Trait with a new generic type parameter that has a bound containing an impl Trait. This transformation stresses the compiler's handling of higher-ranked trait bounds, nested opaque types, and generic parameter interactions, potentially revealing bugs in trait resolution and binder processing."
    }
}

struct ReplaceNestedImplTraitVisitor {
    trait_names: HashSet<String>,
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for ReplaceNestedImplTraitVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::ImplTrait(type_impl_trait) = ty {
            if let Some((outer_path, inner_impl)) = self.extract_nested_pattern(&type_impl_trait.bounds) {
                if let Some(trait_name) = self.trait_names.iter().next() {
                    let new_param_ident = Ident::new("U", Span::call_site());
                    let trait_bound_path: SynPath = parse_quote!(#trait_name);
                    let mut new_bounds = Punctuated::new();
                    new_bounds.push(TypeParamBound::Trait(TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: outer_path.clone(),
                    }));
                    let new_return_type: Type = parse_quote!(#outer_path<#new_param_ident>);
                    *ty = Type::ImplTrait(TypeImplTrait {
                        impl_token: type_impl_trait.impl_token,
                        bounds: new_bounds,
                    });
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

impl ReplaceNestedImplTraitVisitor {
    fn extract_nested_pattern(&self, bounds: &Punctuated<TypeParamBound, token::Plus>) -> Option<(SynPath, TypeImplTrait)> {
        for bound in bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                if let Some(segment) = trait_bound.path.segments.last() {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Type(Type::ImplTrait(inner_impl)) = arg {
                                return Some((trait_bound.path.clone(), inner_impl.clone()));
                            }
                        }
                    }
                }
            }
        }
        None
    }
}