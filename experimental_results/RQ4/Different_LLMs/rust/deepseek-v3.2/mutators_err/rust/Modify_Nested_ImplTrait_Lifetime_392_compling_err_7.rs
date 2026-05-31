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

pub struct Modify_Nested_ImplTrait_Lifetime_392;

impl Mutator for Modify_Nested_ImplTrait_Lifetime_392 {
    fn name(&self) -> &str {
        "Modify_Nested_ImplTrait_Lifetime_392"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = NestedImplTraitLifetimeVisitor::default();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic impl blocks with const generic parameters and associated functions returning nested impl Trait types. It modifies lifetime bindings on the nested impl Trait within the return type's associated type. If an explicit bound exists, it removes or changes it to a different lifetime parameter already in scope. If no bound exists, it adds one using an existing lifetime parameter or introduces a new lifetime parameter to the function signature. This transformation tests the compiler's handling of complex lifetime interactions in nested opaque return types with const generics."
    }
}

#[derive(Default)]
struct NestedImplTraitLifetimeVisitor {
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for NestedImplTraitLifetimeVisitor {
    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        let has_const_generic = i.generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
        if !has_const_generic {
            return;
        }
        for impl_item in &mut i.items {
            if let syn::ImplItem::Fn(func) = impl_item {
                self.process_function(&mut func.sig, &mut func.block);
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, i);
    }
}

impl NestedImplTraitLifetimeVisitor {
    fn process_function(&mut self, sig: &mut syn::Signature, _block: &mut syn::Block) {
        if let syn::ReturnType::Type(_, return_type) = &mut sig.output {
            let mut return_type_clone = (**return_type).clone();
            self.mutate_nested_impl_trait_lifetime(sig, &mut return_type_clone);
            *return_type = Box::new(return_type_clone);
        }
    }

    fn mutate_nested_impl_trait_lifetime(&mut self, sig: &mut syn::Signature, ty: &mut syn::Type) {
        match ty {
            syn::Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_impl_trait.bounds {
                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_trait_bound_mut(sig, trait_bound);
                    }
                }
            }
            syn::Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let syn::GenericArgument::Type(inner_ty) = arg {
                                self.mutate_nested_impl_trait_lifetime(sig, inner_ty);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }

    fn visit_trait_bound_mut(&mut self, sig: &mut syn::Signature, trait_bound: &mut syn::TraitBound) {
        if let syn::PathArguments::AngleBracketed(args) = &mut trait_bound.path.segments.last_mut().unwrap().arguments {
            for arg in &mut args.args {
                if let syn::GenericArgument::AssocType(assoc) = arg {
                    if let syn::Type::ImplTrait(inner_impl_trait) = &mut assoc.ty {
                        self.mutate_inner_impl_trait_lifetime(sig, inner_impl_trait);
                    }
                }
            }
        }
    }

    fn mutate_inner_impl_trait_lifetime(&mut self, sig: &mut syn::Signature, inner_impl_trait: &mut syn::TypeImplTrait) {
        let existing_lifetimes: Vec<Lifetime> = sig.generics.params.iter()
            .filter_map(|p| match p {
                syn::GenericParam::Lifetime(lp) => Some(lp.lifetime.clone()),
                _ => None,
            })
            .collect();

        let has_explicit_bound = inner_impl_trait.bounds.iter().any(|b| matches!(b, syn::TypeParamBound::Lifetime(_)));
        
        if has_explicit_bound {
            if self.rng.gen_bool(0.5) && !existing_lifetimes.is_empty() {
                let chosen_lifetime = existing_lifetimes.choose(&mut self.rng).unwrap().clone();
                inner_impl_trait.bounds = self.replace_or_add_lifetime_bound(&inner_impl_trait.bounds, chosen_lifetime);
            } else {
                inner_impl_trait.bounds = self.remove_lifetime_bounds(&inner_impl_trait.bounds);
            }
        } else {
            if !existing_lifetimes.is_empty() {
                let chosen_lifetime = existing_lifetimes.choose(&mut self.rng).unwrap().clone();
                inner_impl_trait.bounds.push(syn::TypeParamBound::Lifetime(chosen_lifetime));
            } else {
                let new_lifetime_name = format!("'l{}", self.rng.gen_range(0..100));
                let new_lifetime = syn::Lifetime::new(&new_lifetime_name, Span::call_site());
                sig.generics.params.push(parse_quote!(#new_lifetime));
                inner_impl_trait.bounds.push(syn::TypeParamBound::Lifetime(new_lifetime));
            }
        }
    }

    fn replace_or_add_lifetime_bound(&self, bounds: &Punctuated<syn::TypeParamBound, token::Plus>, new_lifetime: syn::Lifetime) -> Punctuated<syn::TypeParamBound, token::Plus> {
        let mut new_bounds = Punctuated::new();
        for bound in bounds {
            match bound {
                syn::TypeParamBound::Lifetime(_) => {
                    new_bounds.push(syn::TypeParamBound::Lifetime(new_lifetime.clone()));
                }
                _ => {
                    new_bounds.push(bound.clone());
                }
            }
        }
        new_bounds
    }

    fn remove_lifetime_bounds(&self, bounds: &Punctuated<syn::TypeParamBound, token::Plus>) -> Punctuated<syn::TypeParamBound, token::Plus> {
        let mut new_bounds = Punctuated::new();
        for bound in bounds {
            if !matches!(bound, syn::TypeParamBound::Lifetime(_)) {
                new_bounds.push(bound.clone());
            }
        }
        new_bounds
    }
}