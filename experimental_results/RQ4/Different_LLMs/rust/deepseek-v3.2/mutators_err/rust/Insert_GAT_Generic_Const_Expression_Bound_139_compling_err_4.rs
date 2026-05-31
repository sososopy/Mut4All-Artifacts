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

pub struct Insert_GAT_Generic_Const_Expression_Bound_139;

impl Mutator for Insert_GAT_Generic_Const_Expression_Bound_139 {
    fn name(&self) -> &str {
        "Insert_GAT_Generic_Const_Expression_Bound_139"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_definitions = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_def) = item {
                trait_definitions.push((trait_def.ident.clone(), trait_def.items.clone()));
            }
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = trait_item {
                        let generics = &mut type_item.generics;
                        let params = generics.params.clone();
                        for param in params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let trait_path = &trait_bound.path;
                                        let trait_items = find_t1rait_items(&trait_definitions, trait_path);
                                        for trait_item in trait_items {
                                            if let TraitItem::Const(const_item) = trait_item {
                                                let const_name = &const_item.ident;
                                                let generic_param_name = &type_param.ident;
                                                let new_predicate: WherePredicate = parse_quote!([(); #generic_param_name::#const_name]:);
                                                if let Some(where_clause) = &mut type_item.generics.where_clause {
                                                    where_clause.predicates.push(new_predicate);
                                                } else {
                                                    let mut predicates = Punctuated::new();
                                                    predicates.push(new_predicate);
                                                    type_item.generics.where_clause = Some(WhereClause {
                                                        where_token: Default::default(),
                                                        predicates,
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
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        let generics = &mut type_item.generics;
                        let params = generics.params.clone();
                        for param in params {
                            if let GenericParam::Type(type_param) = param {
                                for bound in &type_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let trait_path = &trait_bound.path;
                                        let trait_items = find_trait_items(&trait_definitions, trait_path);
                                        for trait_item in trait_items {
                                            if let TraitItem::Const(const_item) = trait_item {
                                                let const_name = &const_item.ident;
                                                let generic_param_name = &type_param.ident;
                                                let new_predicate: WherePredicate = parse_quote!([(); #generic_param_name::#const_name]:);
                                                if let Some(where_clause) = &mut type_item.generics.where_clause {
                                                    where_clause.predicates.push(new_predicate);
                                                } else {
                                                    let mut predicates = Punctuated::new();
                                                    predicates.push(new_predicate);
                                                    type_item.generics.where_clause = Some(WhereClause {
                                                        where_token: Default::default(),
                                                        predicates,
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a generic const expression bound referencing a generic associated type const in a where clause for a GAT definition or its impl. It locates trait definitions with GATs having trait bounds on generic parameters, where the bounded trait includes an associated const. Then, in the GAT's where clause, it adds a bound of the form `[(); T::CONST]:` where T is the GAT's generic parameter and CONST is the associated const from the trait bound. This mutation also applies to corresponding GAT implementations. The transformation stresses the compiler's handling of generic const expressions, associated constants in trait bounds, and where clause validation for GATs, potentially triggering const evaluation errors or trait resolution failures."
    }
}

fn find_trait_items<'a>(trait_definitions: &'a Vec<(Ident, Vec<TraitItem>)>, trait_path: &'a syn::Path) -> Vec<&'a syn::TraitItem> {
    let mut items = Vec::new();
    for (ident, trait_items) in trait_definitions {
        if *ident == trait_path.segments.last().unwrap().ident {
            for trait_item in trait_items {
                items.push(trait_item);
            }
        }
    }
    items
}