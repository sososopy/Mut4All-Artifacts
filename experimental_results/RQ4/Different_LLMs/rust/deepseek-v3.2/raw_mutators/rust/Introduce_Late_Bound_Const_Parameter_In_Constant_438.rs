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

pub struct Introduce_Late_Bound_Const_Parameter_In_Constant_438;

impl Mutator for Introduce_Late_Bound_Const_Parameter_In_Constant_438 {
    fn name(&self) -> &str {
        "Introduce_Late_Bound_Const_Parameter_In_Constant_438"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    self.mutate_generics(&mut item_fn.sig.generics);
                }
                Item::Impl(item_impl) => {
                    self.mutate_generics(&mut item_impl.generics);
                }
                Item::Trait(item_trait) => {
                    self.mutate_generics(&mut item_trait.generics);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where clauses with for<const C: T> binders. It modifies or adds a predicate that references the bound const parameter C within a constant context where late-bound parameters are disallowed, such as an array repeat expression. This transformation aims to trigger internal compiler errors related to the non_lifetime_binders feature by creating a mismatch between early-bound and late-bound const parameter expectations."
    }
}

impl Introduce_Late_Bound_Const_Parameter_In_Constant_438 {
    fn mutate_generics(&self, generics: &mut syn::Generics) {
        if let Some(where_clause) = &mut generics.where_clause {
            let predicates = &mut where_clause.predicates;
            let mut new_predicates = Punctuated::new();
            let mut found_for_const = false;
            let mut const_param_name = None;

            for predicate in predicates.iter() {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Some(BoundLifetimes { lifetimes, .. }) = &pred_type.lifetimes {
                        for lifetime in lifetimes {
                            if let GenericParam::Const(const_param) = &lifetime {
                                found_for_const = true;
                                const_param_name = Some(const_param.ident.clone());
                                break;
                            }
                        }
                    }
                }
            }

            if found_for_const {
                if let Some(const_name) = const_param_name {
                    let mut modified = false;
                    for predicate in predicates.iter_mut() {
                        if let WherePredicate::Type(pred_type) = predicate {
                            self.modify_predicate(pred_type, &const_name);
                            modified = true;
                        }
                    }
                    if !modified {
                        let new_pred = self.create_new_predicate(&const_name);
                        predicates.push(new_pred);
                    }
                }
            } else {
                let const_name = Ident::new("C", Span::call_site());
                let new_pred = self.create_new_predicate(&const_name);
                predicates.push(new_pred);
            }
        } else {
            let const_name = Ident::new("C", Span::call_site());
            let new_pred = self.create_new_predicate(&const_name);
            generics.where_clause = Some(WhereClause {
                where_token: Default::default(),
                predicates: {
                    let mut predicates = Punctuated::new();
                    predicates.push(new_pred);
                    predicates
                },
            });
        }
    }

    fn modify_predicate(&self, pred_type: &mut PredicateType, const_name: &Ident) {
        match &mut pred_type.bounded_ty {
            Type::Array(array) => {
                if let Expr::Lit(expr_lit) = &*array.len {
                    if let Lit::Int(lit_int) = &expr_lit.lit {
                        if lit_int.suffix().is_empty() || lit_int.suffix() == "usize" {
                            let new_len: Expr = parse_quote! { #const_name };
                            array.len = Box::new(new_len);
                        }
                    }
                }
            }
            Type::Path(type_path) => {
                if let Some(last_segment) = type_path.path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Const(expr) = arg {
                                if let Expr::Lit(expr_lit) = &**expr {
                                    if let Lit::Int(lit_int) = &expr_lit.lit {
                                        if lit_int.suffix().is_empty() || lit_int.suffix() == "usize" {
                                            let new_expr: Expr = parse_quote! { #const_name };
                                            *expr = Box::new(new_expr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }

    fn create_new_predicate(&self, const_name: &Ident) -> WherePredicate {
        let array_type: Type = parse_quote! { [(); #const_name] };
        let sized_path: Path = parse_quote! { Sized };
        let sized_bound = TypeParamBound::Trait(TraitBound {
            paren_token: None,
            modifier: TraitBoundModifier::None,
            lifetimes: None,
            path: sized_path,
        });
        let mut bounds = Punctuated::new();
        bounds.push(sized_bound);

        WherePredicate::Type(PredicateType {
            lifetimes: Some(BoundLifetimes {
                for_token: Default::default(),
                lt_token: Default::default(),
                lifetimes: {
                    let mut lifetimes = Punctuated::new();
                    lifetimes.push(GenericParam::Const(ConstParam {
                        attrs: Vec::new(),
                        const_token: Default::default(),
                        ident: const_name.clone(),
                        colon_token: Default::default(),
                        ty: Type::Path(TypePath {
                            qself: None,
                            path: Path::from(Ident::new("usize", Span::call_site())),
                        }),
                        eq_token: None,
                        default: None,
                    }));
                    lifetimes
                },
                gt_token: Default::default(),
            }),
            bounded_ty: array_type,
            colon_token: Default::default(),
            bounds,
        })
    }
}