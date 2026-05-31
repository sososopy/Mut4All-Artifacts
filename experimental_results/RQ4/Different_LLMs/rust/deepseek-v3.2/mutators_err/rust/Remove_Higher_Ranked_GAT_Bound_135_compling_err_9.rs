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

pub struct Remove_Higher_Ranked_GAT_Bound_135;

impl Mutator for Remove_Higher_Ranked_GAT_Bound_135 {
    fn name(&self) -> &str {
        "Remove_Higher_Ranked_GAT_Bound_135"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = HRTBRemoverVisitor {
                    available_lifetimes: Vec::new(),
                };
                visitor.visit_item_fn(item_fn);
                let lifetimes = visitor.available_lifetimes;
                let mut visitor_mut = HRTBRemoverMutator {
                    lifetimes_to_use: lifetimes,
                };
                visitor_mut.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures containing generic associated type references within higher-ranked trait bounds (HRTBs) like `for<'a> fn(T::Assoc<'a>)`. It removes the higher-ranked quantification, replacing it with a concrete lifetime. If a named lifetime parameter exists in the function's generic parameters (excluding 'static'), that lifetime is used; otherwise, 'static is used. This transformation simplifies HRTBs to non-higher-ranked versions, potentially exposing bugs in the compiler's handling of HRTBs combined with generic associated types, such as type-checking errors or internal compiler errors."
    }
}

struct HRTBRemoverVisitor {
    available_lifetimes: Vec<Lifetime>,
}

impl Visit<'_> for HRTBRemoverVisitor {
    fn visit_generic_param(&mut self, node: &GenericParam) {
        if let GenericParam::Lifetime(lifetime_param) = node {
            if lifetime_param.lifetime.ident != "static" {
                self.available_lifetimes.push(lifetime_param.lifetime.clone());
            }
        }
    }
}

struct HRTBRemoverMutator {
    lifetimes_to_use: Vec<Lifetime>,
}

impl VisitMut for HRTBRemoverMutator {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::ImplTrait(type_impl_trait) = node {
            if let Some(bound) = type_impl_trait.bounds.first() {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let Some(bound_lifetimes) = &trait_bound.lifetimes {
                        let replacement_lifetime = if self.lifetimes_to_use.is_empty() {
                            Lifetime::new("'static", Span::call_site())
                        } else {
                            self.lifetimes_to_use[0].clone()
                        };
                        *node = Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: Ident::new("T", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![GenericArgument::Lifetime(replacement_lifetime)]),
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                }]),
                            },
                        });
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}