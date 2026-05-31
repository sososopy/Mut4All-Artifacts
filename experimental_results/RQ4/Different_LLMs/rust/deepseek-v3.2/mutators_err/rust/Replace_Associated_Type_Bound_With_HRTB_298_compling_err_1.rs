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

pub struct Replace_Associated_Type_Bound_With_HRTB_298;

impl Mutator for Replace_Associated_Type_Bound_With_HRTB_298 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_Bound_With_HRTB_298"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_def) = item {
                let mut has_lifetime_assoc = false;
                for trait_item in &trait_def.items {
                    if let TraitItem::Type(assoc_type) = trait_item {
                        if let Some(generics) = &assoc_type.generics {
                            for param in &generics.params {
                                if let GenericParam::Lifetime(_) = param {
                                    has_lifetime_assoc = true;
                                    break;
                                }
                            }
                        }
                    }
                    if has_lifetime_assoc {
                        break;
                    }
                }
                if !has_lifetime_assoc {
                    continue;
                }
                for trait_item in &mut trait_def.items {
                    if let TraitItem::Fn(method) = trait_item {
                        let mut visitor = AssociatedTypeBoundVisitor {
                            method_generics: &mut method.sig.generics,
                            found: false,
                        };
                        visitor.visit_signature_mut(&mut method.sig);
                        if visitor.found {
                            let new_lifetime = Lifetime::new("'mutated", Span::call_site());
                            let mut new_predicates = Punctuated::new();
                            new_predicates.push(WherePredicate::Type(PredicateType {
                                lifetimes: Some(BoundLifetimes {
                                    lifetimes: Punctuated::from_iter(vec![LifetimeParam {
                                        attrs: Vec::new(),
                                        lifetime: new_lifetime.clone(),
                                        colon_token: None,
                                        bounds: Punctuated::new(),
                                    }]),
                                    for_token: token::For::default(),
                                    gt_token: token::Gt::default(),
                                    lt_token: token::Lt::default(),
                                }),
                                bounded_ty: Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: Ident::new("Self", Span::call_site()),
                                                arguments: PathArguments::None,
                                            });
                                            segments.push(PathSegment {
                                                ident: Ident::new("Item", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Lifetime(
                                                                new_lifetime,
                                                            ));
                                                            args
                                                        },
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            });
                                            segments
                                        },
                                    },
                                }),
                                colon_token: token::Colon::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath::from(Ident::new("Copy", Span::call_site())),
                                    }));
                                    bounds
                                },
                            }));
                            if let Some(where_clause) = &mut method.sig.generics.where_clause {
                                where_clause.predicates.extend(new_predicates);
                            } else {
                                method.sig.generics.where_clause = Some(WhereClause {
                                    where_token: token::Where::default(),
                                    predicates: new_predicates,
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with associated types that have lifetime parameters. It identifies method signatures where the associated type is used with a trait bound (e.g., Copy) incorrectly applied via colon syntax in a generic argument. The mutation replaces this usage with a higher-ranked trait bound (HRTB) that introduces a new lifetime parameter 'mutated, moving the bound to a where clause. This transformation aims to expose compiler bugs related to associated type bounds, lifetime interactions, and HRTB handling in trait definitions, potentially causing internal compiler errors or unexpected type resolution failures."
    }
}

struct AssociatedTypeBoundVisitor<'a> {
    method_generics: &'a mut Generics,
    found: bool,
}

impl<'a> VisitMut for AssociatedTypeBoundVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(qself) = &type_path.qself {
                if let Type::Path(qself_ty) = &*qself.ty {
                    if qself_ty.path.is_ident("Self") {
                        for segment in &mut type_path.path.segments {
                            if segment.ident == "Item" {
                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Type(arg_ty) = arg {
                                            if let Type::Path(arg_path) = arg_ty {
                                                for arg_segment in &mut arg_path.path.segments {
                                                    if let PathArguments::AngleBracketed(arg_args) = &mut arg_segment.arguments {
                                                        for arg_arg in &mut arg_args.args {
                                                            if let GenericArgument::Type(arg_arg_ty) = arg_arg {
                                                                if let Type::TraitObject(trait_obj) = arg_arg_ty {
                                                                    if let Some(first_bound) = trait_obj.bounds.first() {
                                                                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                                                                            if trait_bound.path.is_ident("Copy") {
                                                                                self.found = true;
                                                                                *arg_arg_ty = Type::Path(TypePath {
                                                                                    qself: None,
                                                                                    path: SynPath::from(arg_segment.ident.clone()),
                                                                                });
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
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}