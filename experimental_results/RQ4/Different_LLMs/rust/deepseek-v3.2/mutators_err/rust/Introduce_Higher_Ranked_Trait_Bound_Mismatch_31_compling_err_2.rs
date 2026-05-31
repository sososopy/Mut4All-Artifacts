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

pub struct Introduce_Higher_Ranked_Trait_Bound_Mismatch_31;

impl Mutator for Introduce_Higher_Ranked_Trait_Bound_Mismatch_31 {
    fn name(&self) -> &str {
        "Introduce_Higher_Ranked_Trait_Bound_Mismatch_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(lifetimes) = &trait_bound.lifetimes {
                                    if lifetimes.lifetimes.len() > 0 {
                                        let mut found_generic = false;
                                        for param in &item_fn.sig.generics.params {
                                            if let GenericParam::Type(type_param) = param {
                                                found_generic = true;
                                                let mut visitor = CallVisitor {
                                                    generic_name: type_param.ident.clone(),
                                                    changed: false,
                                                };
                                                visitor.visit_block_mut(&mut item_fn.block);
                                                if visitor.changed {
                                                    break;
                                                }
                                            }
                                        }
                                        if !found_generic {
                                            item_fn.sig.generics.params.push(parse_quote!(T: Clone = ()));
                                            let mut visitor = CallVisitor {
                                                generic_name: Ident::new("T", Span::call_site()),
                                                changed: false,
                                            };
                                            visitor.visit_block_mut(&mut item_fn.block);
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
        "The mutation operator targets functions returning impl Trait with higher-ranked lifetime bounds and associated types. It identifies such functions and modifies their bodies by substituting concrete type arguments in call expressions with a generic type parameter, either existing or newly added. This introduces a generic parameter into the associated type projection, potentially causing region solver failures when equating higher-ranked lifetimes from the impl Trait return type with those from nested associated type projections involving the generic type."
    }
}

struct CallVisitor {
    generic_name: Ident,
    changed: bool,
}

impl VisitMut for CallVisitor {
    fn visit_expr_call_mut(&mut self, expr: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*expr.func {
            if path.segments.last().map(|seg| seg.ident.to_string()) == Some("always".to_string()) {
                if expr.args.len() == 1 {
                    let mut new_path = path.clone();
                    new_path.segments.last_mut().unwrap().arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(self.generic_name.clone()),
                        }))]),
                        gt_token: token::Gt::default(),
                    });
                    expr.func = Box::new(Expr::Path(ExprPath { path: new_path, attrs: vec![] }));
                    self.changed = true;
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr);
    }
}