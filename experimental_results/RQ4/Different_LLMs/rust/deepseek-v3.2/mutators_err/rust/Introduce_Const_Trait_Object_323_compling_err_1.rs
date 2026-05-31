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

pub struct Introduce_Const_Trait_Object_323;

impl Mutator for Introduce_Const_Trait_Object_323 {
    fn name(&self) -> &str {
        "Introduce_Const_Trait_Object_323"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        let mut auto_traits = HashSet::new();
        auto_traits.insert("Send");
        auto_traits.insert("Sync");
        auto_traits.insert("Unpin");
        auto_traits.insert("UnwindSafe");
        auto_traits.insert("RefUnwindSafe");

        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let trait_name = item_trait.ident.to_string();
                if !auto_traits.contains(&trait_name.as_str()) {
                    traits.push(item_trait.ident.clone());
                }
            }
        }

        let mut visitor = DynTraitVisitor {
            traits: &traits,
            auto_traits: &auto_traits,
            rng: &mut thread_rng(),
        };
        visitor.visit_file_mut(file);

        if !traits.is_empty() && !visitor.found_dyn {
            let target_trait = traits.choose(&mut thread_rng()).unwrap();
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    let new_stmt: Stmt = parse_quote! {
                        let _: &dyn ~const #target_trait;
                    };
                    item_fn.block.stmts.insert(0, new_stmt);
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces `~const` trait bounds into dyn trait objects. It first scans for existing dyn trait objects and modifies them by adding `~const` before non-auto traits. If no dyn trait objects exist, it creates a new variable with a dyn ~const trait object referencing an existing non-auto trait. This transformation targets the compiler's handling of const trait bounds in trait objects, potentially exposing bugs in type lowering, object safety checking, or projection mismatches when the const_trait_impl feature is not enabled."
    }
}

struct DynTraitVisitor<'a> {
    traits: &'a Vec<Ident>,
    auto_traits: &'a HashSet<&'a str>,
    rng: &'a mut rand::rngs::ThreadRng,
    found_dyn: bool,
}

impl<'a> VisitMut for DynTraitVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::TraitObject(trait_obj) = ty {
            self.found_dyn = true;
            let mut new_bounds = Punctuated::new();
            for bound in &trait_obj.bounds {
                match bound {
                    TypeParamBound::Trait(trait_bound) => {
                        let path = &trait_bound.path;
                        if let Some(segment) = path.segments.last() {
                            let trait_name = segment.ident.to_string();
                            if !self.auto_traits.contains(&trait_name.as_str()) && self.rng.gen_bool(0.5) {
                                let mut new_trait_bound = trait_bound.clone();
                                new_trait_bound.modifier = TraitBoundModifier::MaybeConst;
                                new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            } else {
                                new_bounds.push(bound.clone());
                            }
                        } else {
                            new_bounds.push(bound.clone());
                        }
                    }
                    _ => new_bounds.push(bound.clone()),
                }
            }
            trait_obj.bounds = new_bounds;
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        if i.sig.ident == "main" {
            return;
        }
        for input in &mut i.sig.inputs {
            if let FnArg::Typed(pat_type) = input {
                self.visit_type_mut(&mut pat_type.ty);
            }
        }
        if let ReturnType::Type(_, return_type) = &mut i.sig.output {
            self.visit_type_mut(return_type);
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Some((_, ty)) = &mut local.ty {
            self.visit_type_mut(ty);
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
}