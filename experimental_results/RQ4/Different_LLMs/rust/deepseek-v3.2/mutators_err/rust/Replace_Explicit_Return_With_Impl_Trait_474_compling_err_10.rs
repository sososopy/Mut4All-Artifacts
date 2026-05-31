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

pub struct Replace_Explicit_Return_With_Impl_Trait_474;

impl Mutator for Replace_Explicit_Return_With_Impl_Trait_474 {
    fn name(&self) -> &str {
        "Replace_Explicit_Return_With_Impl_Trait_474"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReturnTypeVisitor {
            changes: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for (span, bounds) in visitor.changes {
            let mut new_bounds = Punctuated::new();
            for bound in bounds {
                new_bounds.push(TypeParamBound::Trait(bound));
            }
            let impl_trait = TypeImplTrait {
                impl_token: token::Impl { span },
                bounds: new_bounds,
            };
            let mut visitor2 = ApplyImplTraitVisitor {
                target_span: span,
                impl_trait: Type::ImplTrait(impl_trait),
            };
            visitor2.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces explicit concrete return types with `impl Trait` where the original type implements at least one trait. For async functions, it uses `impl Future<Output = ...>`. It selects the most contextually appropriate trait (Future, Iterator, Fn, or common traits like Debug, Display). This transformation tests the compiler's handling of opaque return types, trait resolution, and async/await type inference under altered signatures while preserving function body semantics."
    }
}

struct ReturnTypeVisitor {
    changes: Vec<(Span, Vec<TraitBound>)>,
}

impl VisitMut for ReturnTypeVisitor {
    fn visit_item_fn_mut(&mut self, node: &mut ItemFn) {
        if let ReturnType::Type(_, box_type) = &mut node.sig.output {
            if !matches!(**box_type, Type::ImplTrait(_)) {
                if let Some(bounds) = infer_trait_bounds(&box_type, &node.sig.asyncness) {
                    self.changes.push((box_type.span(), bounds));
                }
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, node);
    }
    fn visit_impl_item_fn_mut(&mut self, node: &mut syn::ImplItemFn) {
        if let ReturnType::Type(_, box_type) = &mut node.sig.output {
            if !matches!(**box_type, Type::ImplTrait(_)) {
                if let Some(bounds) = infer_trait_bounds(&box_type, &node.sig.asyncness) {
                    self.changes.push((box_type.span(), bounds));
                }
            }
        }
        syn::visit_mut::visit_impl_item_fn_mut(self, node);
    }
    fn visit_item_trait_mut(&mut self, node: &mut syn::ItemTrait) {
        for item in &mut node.items {
            if let syn::TraitItem::Fn(method) = item {
                if let ReturnType::Type(_, box_type) = &mut method.sig.output {
                    if !matches!(**box_type, Type::ImplTrait(_)) {
                        if let Some(bounds) = infer_trait_bounds(&box_type, &method.sig.asyncness) {
                            self.changes.push((box_type.span(), bounds));
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_item_trait_mut(self, node);
    }
}

struct ApplyImplTraitVisitor {
    target_span: Span,
    impl_trait: Type,
}

impl VisitMut for ApplyImplTraitVisitor {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if node.span().into() == self.target_span.into() {
            *node = self.impl_trait.clone();
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}

fn infer_trait_bounds(ty: &Type, asyncness: &Option<token::Async>) -> Option<Vec<TraitBound>> {
    if asyncness.is_some() {
        if let Type::Path(TypePath { path, .. }) = ty {
            let output_type = ty.clone();
            let future_bound = TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: parse_quote!(std::future::Future<Output = #output_type>),
            };
            return Some(vec![future_bound]);
        }
    }
    let mut candidates = Vec::new();
    if let Type::Path(TypePath { path, .. }) = ty {
        let last_segment = path.segments.last()?;
        let ident_str = last_segment.ident.to_string();
        match ident_str.as_str() {
            "Vec" | "Slice" | "Range" => {
                candidates.push(parse_quote!(Iterator));
            }
            "String" | "str" => {
                candidates.push(parse_quote!(std::fmt::Display));
                candidates.push(parse_quote!(std::fmt::Debug));
            }
            "Result" | "Option" => {
                candidates.push(parse_quote!(std::fmt::Debug));
            }
            "HashMap" | "BTreeMap" => {
                candidates.push(parse_quote!(std::fmt::Debug));
            }
            "Box" | "Rc" | "Arc" => {
                candidates.push(parse_quote!(std::fmt::Debug));
            }
            _ => {
                candidates.push(parse_quote!(std::fmt::Debug));
                candidates.push(parse_quote!(std::fmt::Display));
                candidates.push(parse_quote!(Iterator));
            }
        }
    } else if let Type::Reference(_) = ty {
        candidates.push(parse_quote!(std::fmt::Debug));
    } else if let Type::Array(_) = ty {
        candidates.push(parse_quote!(std::fmt::Debug));
    } else if let Type::Tuple(_) = ty {
        candidates.push(parse_quote!(std::fmt::Debug));
    } else {
        candidates.push(parse_quote!(std::fmt::Debug));
    }
    if !candidates.is_empty() {
        Some(candidates.into_iter().map(|path| TraitBound {
            paren_token: None,
            modifier: TraitBoundModifier::None,
            lifetimes: None,
            path,
        }).collect())
    } else {
        None
    }
}