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

pub struct Replace_Trait_Bound_With_HRTB_Escape_445;

impl Mutator for Replace_Trait_Bound_With_HRTB_Escape_445 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_HRTB_Escape_445"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = HRTBMutationVisitor::new();
        visitor.visit_file_mut(file);
        if visitor.needs_simple_trait {
            file.items.insert(0, parse_quote! {
                trait Simple {}
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing trait bounds with higher-ranked trait bounds (HRTB) using for<...> syntax, placing bound variables in nested contexts where they may escape, such as inside associated type projections or other trait bounds. This transformation targets function signatures, trait definitions, and where clauses, aiming to stress the compiler's binder infrastructure and potentially cause escaping bound variable errors or lifetime resolution failures."
    }
}

struct HRTBMutationVisitor {
    needs_simple_trait: bool,
}

impl HRTBMutationVisitor {
    fn new() -> Self {
        Self { needs_simple_trait: false }
    }

    fn mutate_trait_bound(&mut self, bound: &mut TypeParamBound) {
        if let TypeParamBound::Trait(trait_bound) = bound {
            let path_clone = trait_bound.path.clone();
            let mut rng = thread_rng();
            let binder_name = format!("V{}", rng.gen_range(0..1000));
            let binder_ident = Ident::new(&binder_name, Span::call_site());

            let mut inner_bound = trait_bound.clone();
            inner_bound.lifetimes = Some(BoundLifetimes {
                for_token: token::For::default(),
                lt_token: token::Lt::default(),
                lifetimes: Punctuated::<GenericParam, Comma>::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new(&format!("'{}", binder_name), Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                })]),
                gt_token: token::Gt::default(),
            });

            let nested_type = if rng.gen_bool(0.5) {
                let simple_trait_path = syn::Path::from(Ident::new("Simple", Span::call_site()));
                self.needs_simple_trait = true;
                parse_quote! { impl Simple<#binder_ident> }
            } else {
                let existing_trait_path = path_clone.segments.last().unwrap().clone();
                parse_quote! { impl #existing_trait_path<#binder_ident> }
            };

            if let Some(path_segment) = trait_bound.path.segments.last_mut() {
                match &mut path_segment.arguments {
                    PathArguments::AngleBracketed(args) => {
                        if !args.args.is_empty() {
                            let last_arg = args.args.last_mut().unwrap();
                            if let GenericArgument::Type(_) = last_arg {
                                *last_arg = GenericArgument::AssocType(syn::AssocType {
                                    ident: Ident::new("Assoc", Span::call_site()),
                                    generics: None,
                                    eq_token: token::Eq::default(),
                                    ty: nested_type,
                                });
                            }
                        } else {
                            args.args.push(GenericArgument::AssocType(syn::AssocType {
                                ident: Ident::new("Assoc", Span::call_site()),
                                generics: None,
                                eq_token: token::Eq::default(),
                                ty: nested_type,
                            }));
                        }
                    }
                    PathArguments::None => {
                        path_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: Punctuated::from_iter(vec![GenericArgument::AssocType(syn::AssocType {
                                ident: Ident::new("Assoc", Span::call_site()),
                                generics: None,
                                eq_token: token::Eq::default(),
                                ty: nested_type,
                            })]),
                            gt_token: token::Gt::default(),
                        });
                    }
                    _ => {}
                }
            }

            *bound = TypeParamBound::Trait(TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: path_clone,
            });
        }
    }
}

impl VisitMut for HRTBMutationVisitor {
    fn visit_type_param_bound_mut(&mut self, bound: &mut TypeParamBound) {
        self.mutate_trait_bound(bound);
        syn::visit_mut::visit_type_param_bound_mut(self, bound);
    }

    fn visit_where_predicate_mut(&mut self, predicate: &mut WherePredicate) {
        if let WherePredicate::Type(pred_type) = predicate {
            for bound in &mut pred_type.bounds {
                self.mutate_trait_bound(bound);
            }
        }
        syn::visit_mut::visit_where_predicate_mut(self, predicate);
    }

    fn visit_type_impl_trait_mut(&mut self, type_impl_trait: &mut TypeImplTrait) {
        for bound in &mut type_impl_trait.bounds {
            self.mutate_trait_bound(bound);
        }
        syn::visit_mut::visit_type_impl_trait_mut(self, type_impl_trait);
    }

    fn visit_trait_item_mut(&mut self, item: &mut TraitItem) {
        if let TraitItem::Fn(trait_fn) = item {
            for bound in &mut trait_fn.sig.generics.params {
                if let GenericParam::Type(type_param) = bound {
                    for bound in &mut type_param.bounds {
                        self.mutate_trait_bound(bound);
                    }
                }
            }
        }
        syn::visit_mut::visit_trait_item_mut(self, item);
    }
}