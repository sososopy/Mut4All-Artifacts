use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, ForeignItem, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, TraitItem, Type, TypeImplTrait,
    TypeParamBound, TypePath, WhereClause, WherePredicate, parse_quote,
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

pub struct Introduce_Associated_Type_Projection_From_Extern_Type_14;

impl Mutator for Introduce_Associated_Type_Projection_From_Extern_Type_14 {
    fn name(&self) -> &str {
        "Introduce_Associated_Type_Projection_From_Extern_Type_14"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut extern_type_names = HashSet::new();
        let mut trait_names_with_associated_type = HashSet::new();
        for item in &file.items {
            if let Item::ForeignMod(foreign_mod) = item {
                for foreign_item in &foreign_mod.items {
                    if let ForeignItem::Type(foreign_type) = foreign_item {
                        extern_type_names.insert(foreign_type.ident.clone());
                    }
                }
            }
            if let Item::Trait(trait_def) = item {
                for trait_item in &trait_def.items {
                    if let TraitItem::Type(assoc_type) = trait_item {
                        trait_names_with_associated_type.insert(trait_def.ident.clone());
                    }
                }
            }
        }
        if extern_type_names.is_empty() || trait_names_with_associated_type.is_empty() {
            return;
        }
        let extern_type_name = extern_type_names.iter().next().unwrap();
        let trait_name = trait_names_with_associated_type.iter().next().unwrap();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let attrs = &func.attrs;
                let is_inline = attrs.iter().any(|attr| {
                    attr.path().segments.iter().any(|seg| seg.ident == "inline")
                });
                if !is_inline {
                    continue;
                }
                let mut lifetime_param_exists = false;
                let mut lifetime_param_name = Ident::new("a", Span::call_site());
                for param in &func.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = param {
                        lifetime_param_exists = true;
                        lifetime_param_name = lifetime_param.lifetime.ident.clone();
                        break;
                    }
                }
                if !lifetime_param_exists {
                    func.sig.generics.params.push(parse_quote!('a));
                }
                let where_clause = &mut func.sig.generics.where_clause;
                let new_predicate: WherePredicate = parse_quote!(&'a #extern_type_name: #trait_name);
                if let Some(where_clause) = where_clause {
                    where_clause.predicates.push(new_predicate);
                } else {
                    func.sig.generics.where_clause = Some(WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::from_iter(vec![new_predicate]),
                    });
                }
                let new_param_type: Type = parse_quote!(<&'a #extern_type_name as #trait_name>::Type);
                let new_param: FnArg = parse_quote!(_p: #new_param_type);
                func.sig.inputs.push(new_param);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline functions in a seed program that contains both an extern type declaration and a trait with an associated type. It adds a lifetime parameter 'a if not present, then appends a where-clause `&'a ExternType: Trait` and a new parameter `_p: <&'a ExternType as Trait>::Type`. This creates an associated type projection from a reference to an extern type within a where-clause, aiming to trigger normalization failures during compilation, especially in inline functions, as per the described compiler bug scenario."
    }
}