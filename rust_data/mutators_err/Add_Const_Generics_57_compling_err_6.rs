use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Add_Const_Generics_57;

impl Mutator for Add_Const_Generics_57 {
    fn name(&self) -> &str {
        "Add_Const_Generics_57"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(where_clause) = &mut item_struct.generics.where_clause {
                    let mut has_const_generic = false;
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &predicate_type.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.segments.last().unwrap().ident == "Sized" {
                                        has_const_generic = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if has_const_generic {
                        let const_generic_1: GenericParam = parse_quote! { const N1: usize };
                        let const_generic_2: GenericParam = parse_quote! { const N2: usize };
                        item_struct.generics.params.push(const_generic_1);
                        item_struct.generics.params.push(const_generic_2);

                        for predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = predicate {
                                if let syn::Type::Array(type_array) = &predicate_type.bounded_ty {
                                    if let syn::Expr::Path(expr_path) = &type_array.len {
                                        let new_expr: ExprPath = parse_quote! { N1 + N2 };
                                        let mut new_len = type_array.len.clone();
                                        if let syn::Expr::Path(expr_path_mut) = &mut new_len {
                                            *expr_path_mut = new_expr;
                                        }
                                        predicate_type.bounded_ty = syn::Type::Array(syn::TypeArray {
                                            len: Box::new(new_len),
                                            ..type_array.clone()
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic struct declarations that use const generics. It introduces two new const generic parameters, N1 and N2, and modifies the where clause to use N1 + N2. This ensures the struct is used with valid constant expressions, maintaining compatibility with the original code."
    }
}