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

pub struct Inherent_Associated_Type_Lifetime_Parameter_Swap_420;

impl Mutator for Inherent_Associated_Type_Lifetime_Parameter_Swap_420 {
    fn name(&self) -> &str {
        "Inherent_Associated_Type_Lifetime_Parameter_Swap_420"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let lifetime_params: Vec<LifetimeParam> = item_impl.generics.params.iter()
                    .filter_map(|param| match param {
                        GenericParam::Lifetime(lp) => Some(lp.clone()),
                        _ => None,
                    })
                    .collect();
                if lifetime_params.len() < 2 {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        let mut visitor = LifetimeSwapper {
                            lifetimes: &lifetime_params,
                            rng: &mut rng,
                            changed: false,
                        };
                        visitor.visit_type_mut(&mut type_item.ty);
                        if visitor.changed {
                            type_item.ty = visitor.ty_result.unwrap();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated type definitions within impl blocks that have at least two lifetime parameters. It swaps the lifetime parameter used in the associated type's definition with a different lifetime parameter from the same scope, potentially creating a mismatch that stresses the substitution and region checking logic. This directly alters the lifetime binding in the IAT, which can lead to region substitution errors similar to the reported bug when the IAT is later referenced in contexts with conflicting lifetime bounds."
    }
}

struct LifetimeSwapper<'a, R: Rng> {
    lifetimes: &'a [LifetimeParam],
    rng: &'a mut R,
    changed: bool,
    ty_result: Option<Box<Type>>,
}

impl<'a, R: Rng> VisitMut for LifetimeSwapper<'a, R> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::Reference(type_ref) => {
                if let Some(lifetime) = &mut type_ref.lifetime {
                    self.visit_lifetime_mut(lifetime);
                }
                self.visit_type_mut(&mut *type_ref.elem);
            }
            Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                self.visit_lifetime_mut(lifetime);
                            }
                        }
                    }
                }
            }
            Type::Slice(type_slice) => {
                self.visit_type_mut(&mut *type_slice.elem);
            }
            Type::Array(type_array) => {
                self.visit_type_mut(&mut *type_array.elem);
            }
            Type::Ptr(type_ptr) => {
                self.visit_type_mut(&mut *type_ptr.elem);
            }
            Type::Tuple(type_tuple) => {
                for elem in &mut type_tuple.elems {
                    self.visit_type_mut(elem);
                }
            }
            Type::BareFn(type_fn) => {
                if let Some(lifetimes) = &mut type_fn.lifetimes {
                    for lifetime_param in &mut lifetimes.lifetimes {
                        self.visit_lifetime_mut(&mut lifetime_param.lifetime);
                    }
                }
                for arg in &mut type_fn.inputs {
                    self.visit_type_mut(&mut arg.ty);
                }
                self.visit_type_mut(&mut *type_fn.output);
            }
            Type::TraitObject(type_trait_obj) => {
                for bound in &mut type_trait_obj.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(lifetimes) = &mut trait_bound.lifetimes {
                            for lifetime_param in &mut lifetimes.lifetimes {
                                self.visit_lifetime_mut(&mut lifetime_param.lifetime);
                            }
                        }
                    }
                }
            }
            Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(lifetimes) = &mut trait_bound.lifetimes {
                            for lifetime_param in &mut lifetimes.lifetimes {
                                self.visit_lifetime_mut(&mut lifetime_param.lifetime);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }

    fn visit_lifetime_mut(&mut self, lifetime: &mut Lifetime) {
        if self.lifetimes.is_empty() {
            return;
        }
        let lifetime_ident = lifetime.ident.to_string();
        let candidates: Vec<&LifetimeParam> = self.lifetimes.iter()
            .filter(|lp| lp.lifetime.ident.to_string() != lifetime_ident)
            .collect();
        if candidates.is_empty() {
            return;
        }
        if self.rng.gen_bool(0.5) {
            let new_lifetime = candidates.choose(self.rng).unwrap();
            lifetime.ident = new_lifetime.lifetime.ident.clone();
            self.changed = true;
        }
    }
}