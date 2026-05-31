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

pub struct Replace_RPITIT_Lifetime_In_Assoc_Type_163;

impl Mutator for Replace_RPITIT_Lifetime_In_Assoc_Type_163 {
    fn name(&self) -> &str {
        "Replace_RPITIT_Lifetime_In_Assoc_Type_163"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut lifetime_names = HashSet::new();
        collect_lifetimes(file, &mut lifetime_names);
        let mut rng = thread_rng();
        let items = file.items.clone();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some(trait_ref) = &item_impl.trait_ {
                    let trait_path = &trait_ref.1;
                    let mut trait_info = TraitInfo::default();
                    find_trait_info(&items, trait_path, &mut trait_info);
                    if trait_info.has_rpitit_method && trait_info.has_assoc_type_with_lifetime {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                        let mut visitor = RPITITVisitor {
                                            impl_lifetimes: extract_lifetimes_from_generics(&item_impl.generics),
                                            method_lifetimes: extract_lifetimes_from_generics(&method.sig.generics),
                                            hrtb_lifetimes: extract_hrtb_lifetimes(&type_impl_trait.bounds),
                                            lifetime_names: &lifetime_names,
                                            rng: &mut rng,
                                            changed: false,
                                        };
                                        let mut return_type_clone = (**return_type).clone();
                                        visitor.visit_type_mut(&mut return_type_clone);
                                        if visitor.changed {
                                            method.sig.output = ReturnType::Type(Default::default(), Box::new(return_type_clone));
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with RPITIT methods and associated types with lifetime parameters. It identifies impl blocks implementing traits that have RPITIT methods and associated types with lifetimes. For such methods, it examines the RPITIT return type and replaces any lifetime argument used in an associated type reference with an undeclared lifetime (e.g., 'missing or 'existing_invalid). This creates a mismatch between the lifetime used and the lifetimes in scope, potentially exposing bugs in RPITIT refinement checking related to regions."
    }
}

struct RPITITVisitor<'a> {
    impl_lifetimes: HashSet<String>,
    method_lifetimes: HashSet<String>,
    hrtb_lifetimes: HashSet<String>,
    lifetime_names: &'a HashSet<String>,
    rng: &'a mut rand::rngs::ThreadRng,
    changed: bool,
}

impl<'a> VisitMut for RPITITVisitor<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if let Some(last_segment) = type_path.path.segments.last_mut() {
                if last_segment.ident == "Self" {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                let lifetime_str = lifetime.ident.to_string();
                                if self.impl_lifetimes.contains(&lifetime_str) ||
                                   self.method_lifetimes.contains(&lifetime_str) ||
                                   self.hrtb_lifetimes.contains(&lifetime_str) {
                                    let new_lifetime_name = if self.lifetime_names.is_empty() {
                                        "missing".to_string()
                                    } else {
                                        let candidates = self.lifetime_names.iter().collect::<Vec<_>>();
                                        let chosen = candidates.choose(self.rng).unwrap();
                                        format!("{}_invalid", chosen)
                                    };
                                    lifetime.ident = Ident::new(&new_lifetime_name, lifetime.ident.span());
                                    self.changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}

fn collect_lifetimes(file: &syn::File, set: &mut HashSet<String>) {
    struct LifetimeCollector<'a> {
        set: &'a mut HashSet<String>,
    }
    impl<'a> Visit<'a> for LifetimeCollector<'a> {
        fn visit_lifetime(&mut self, node: &'a Lifetime) {
            self.set.insert(node.ident.to_string());
        }
    }
    let mut collector = LifetimeCollector { set };
    collector.visit_file(file);
}

fn extract_lifetimes_from_generics(generics: &Generics) -> HashSet<String> {
    let mut lifetimes = HashSet::new();
    for param in &generics.params {
        if let GenericParam::Lifetime(lifetime_param) = param {
            lifetimes.insert(lifetime_param.lifetime.ident.to_string());
        }
    }
    lifetimes
}

fn extract_hrtb_lifetimes(bounds: &Punctuated<TypeParamBound, token::Plus>) -> HashSet<String> {
    let mut lifetimes = HashSet::new();
    for bound in bounds {
        if let TypeParamBound::Trait(trait_bound) = bound {
            if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                for lifetime_param in &bound_lifetimes.lifetimes {
                    lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                }
            }
        }
    }
    lifetimes
}

struct TraitInfo {
    has_rpitit_method: bool,
    has_assoc_type_with_lifetime: bool,
}

impl Default for TraitInfo {
    fn default() -> Self {
        TraitInfo {
            has_rpitit_method: false,
            has_assoc_type_with_lifetime: false,
        }
    }
}

fn find_trait_info(file: &syn::File, trait_path: &syn::Path, info: &mut TraitInfo) {
    for item in &file.items {
        if let Item::Trait(item_trait) = item {
            if item_trait.ident == trait_path.segments.last().unwrap().ident {
                for trait_item in &item_trait.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(_) = &**return_type {
                                info.has_rpitit_method = true;
                            }
                        }
                    }
                    if let syn::TraitItem::Type(type_item) = trait_item {
                        for bound in &type_item.bounds {
                            if let TypeParamBound::Lifetime(_) = bound {
                                info.has_assoc_type_with_lifetime = true;
                            }
                        }
                    }
                }
            }
        }
    }
}