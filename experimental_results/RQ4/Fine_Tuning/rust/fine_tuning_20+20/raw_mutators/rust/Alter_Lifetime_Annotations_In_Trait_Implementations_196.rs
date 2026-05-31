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

pub struct Alter_Lifetime_Annotations_In_Trait_Implementations_196;

impl Mutator for Alter_Lifetime_Annotations_In_Trait_Implementations_196 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_In_Trait_Implementations_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let mut lifetimes = HashSet::new();
                    for seg in &path.segments {
                        if let PathArguments::AngleBracketed(args) = &seg.arguments {
                            for arg in &args.args {
                                if let GenericArgument::Lifetime(lt) = arg {
                                    lifetimes.insert(lt.ident.to_string());
                                }
                            }
                        }
                    }
                    if lifetimes.is_empty() {
                        continue;
                    }
                    let mut target_lifetime = None;
                    for lifetime in &item_impl.generics.params {
                        if let GenericParam::Lifetime(lt) = lifetime {
                            if lifetimes.contains(&lt.lifetime.ident.to_string()) {
                                target_lifetime = Some(lt.lifetime.clone());
                                break;
                            }
                        }
                    }
                    if target_lifetime.is_none() {
                        continue;
                    }
                    let target_lifetime = target_lifetime.unwrap();
                    let mut used_lifetimes = HashSet::new();
                    for impl_item in &item_impl.items {
                        if let ImplItem::Type(impl_type) = impl_item {
                            let mut visitor = LifetimeCollector {
                                lifetimes: HashSet::new(),
                            };
                            visitor.visit_type(&impl_type.ty);
                            for lt in visitor.lifetimes {
                                used_lifetimes.insert(lt);
                            }
                        }
                    }
                    let mut new_lifetime_name = None;
                    let mut counter = 0;
                    loop {
                        let candidate = if counter == 0 {
                            "b".to_string()
                        } else {
                            format!("b{}", counter)
                        };
                        if !used_lifetimes.contains(&candidate) {
                            new_lifetime_name = Some(candidate);
                            break;
                        }
                        counter += 1;
                    }
                    let new_lifetime_name = new_lifetime_name.unwrap();
                    let mut new_items = Vec::new();
                    for impl_item in &item_impl.items {
                        if let ImplItem::Type(impl_type) = impl_item {
                            let mut replacer = LifetimeReplacer {
                                target: target_lifetime.clone(),
                                replacement: Lifetime::new(&new_lifetime_name, Span::call_site()),
                            };
                            let mut new_impl_type = impl_type.clone();
                            replacer.visit_type_mut(&mut new_impl_type.ty);
                            new_items.push(ImplItem::Type(new_impl_type));
                        } else {
                            new_items.push(impl_item.clone());
                        }
                    }
                    item_impl.items = new_items;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations with lifetime annotations. It identifies the lifetime parameter used in the trait reference and replaces it with a new, undeclared lifetime in the associated type definitions. This introduces a lifetime mismatch, challenging the compiler's ability to resolve and check lifetimes in trait implementations."
    }
}

struct LifetimeCollector {
    lifetimes: HashSet<String>,
}

impl<'ast> Visit<'ast> for LifetimeCollector {
    fn visit_lifetime(&mut self, i: &'ast Lifetime) {
        self.lifetimes.insert(i.ident.to_string());
    }
}

struct LifetimeReplacer {
    target: Lifetime,
    replacement: Lifetime,
}

impl VisitMut for LifetimeReplacer {
    fn visit_lifetime_mut(&mut self, i: &mut Lifetime) {
        if i.ident == self.target.ident {
            *i = self.replacement.clone();
        }
    }
}