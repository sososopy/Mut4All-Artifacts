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

pub struct Insert_Unsized_In_Array_Length_190;

impl Mutator for Insert_Unsized_In_Array_Length_190 {
    fn name(&self) -> &str {
        "Insert_Unsized_In_Array_Length_190"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_trivial_bounds_feature = false;
        for attr in &file.attrs {
            if let syn::Meta::List(list) = &attr.meta {
                if list.path.is_ident("feature") {
                    for nested in list.nested.iter() {
                        if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                            if path.is_ident("trivial_bounds") {
                                has_trivial_bounds_feature = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if !has_trivial_bounds_feature {
            return;
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut unsized_type = None;
                if let Some(where_clause) = &func.sig.generics.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(pred) = predicate {
                            if let Type::Path(type_path) = &pred.bounded_ty {
                                if type_path.path.segments.len() == 1 {
                                    let segment = &type_path.path.segments[0];
                                    if segment.ident == "str" || segment.ident.to_string().starts_with("dyn") {
                                        for bound in &pred.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                if trait_bound.path.is_ident("Sized") {
                                                    unsized_type = Some(Type::Path(type_path.clone()));
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if unsized_type.is_none() {
                    unsized_type = Some(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(Ident::new("str", Span::call_site())),
                    }));
                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        where_clause.predicates.push(parse_quote!(str: Sized));
                    } else {
                        func.sig.generics.where_clause = Some(parse_quote!(where str: Sized));
                    }
                }

                let unsized_type = unsized_type.unwrap();
                let mut visitor = ArrayLengthVisitor {
                    unsized_type: unsized_type,
                    changes_made: false,
                };
                visitor.visit_file_mut(file);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array types with block length expressions in functions where the trivial_bounds feature is enabled. It ensures a trivial bound for an unsized type (like str) exists in the where clause, then inserts a dummy variable declaration using that unsized type inside the array length block. This transformation aims to trigger layout computation errors by forcing the compiler to evaluate unsized types in const contexts, exploiting the mismatch between trivial bounds and actual unsized type behavior."
    }
}

struct ArrayLengthVisitor {
    unsized_type: Type,
    changes_made: bool,
}

impl VisitMut for ArrayLengthVisitor {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(arr) = node {
            if let Expr::Block(block) = &arr.len {
                let new_stmt: Stmt = parse_quote! {
                    let _dummy: Option<#self.unsized_type> = None;
                };
                let mut new_block = block.clone();
                new_block.block.stmts.insert(0, new_stmt);
                arr.len = Expr::Block(new_block);
                self.changes_made = true;
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}