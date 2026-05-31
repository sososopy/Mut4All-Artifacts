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

pub struct Add_Unsatisfiable_Union_Const_Generic_Constraint_351;

impl Mutator for Add_Unsatisfiable_Union_Const_Generic_Constraint_351 {
    fn name(&self) -> &str {
        "Add_Unsatisfiable_Union_Const_Generic_Constraint_351"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Union(item_union) = item {
                if let Some(where_clause) = &mut item_union.generics.where_clause {
                    let mut new_predicates = where_clause.predicates.clone();
                    let mut has_const_generic_predicate = false;
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let syn::Type::Path(type_path) = &predicate_type.bounded_ty {
                                if type_path.qself.is_none()
                                    && type_path.path.leading_colon.is_none()
                                    && type_path.path.segments.len() == 1
                                    && type_path.path.segments[0].ident == ""
                                {
                                    has_const_generic_predicate = true;
                                    break;
                                }
                            }
                        }
                    }
                    if has_const_generic_predicate {
                        new_predicates.push(parse_quote!([(); N * M]:));
                        item_union.generics.where_clause =
                            Some(syn::WhereClause { predicates: new_predicates, ..*where_clause });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets unions with const generic parameters and existing const-based where clauses. It injects an additional where clause that involves a new arithmetic operation on the const parameters, such as multiplication, to create an unsatisfiable or conflicting constraint. This transformation aims to exploit the complexity of const generics and trigger errors related to constraint validation, size computation, or type resolution within the union context."
    }
}