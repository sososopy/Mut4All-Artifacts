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

pub struct Insert_False_Sized_Bound_With_Const_79;

impl Mutator for Insert_False_Sized_Bound_With_Const_79 {
    fn name(&self) -> &str {
        "Insert_False_Sized_Bound_With_Const_79"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut unsized_types = Vec::new();
                let mut visitor = UnsizedTypeVisitor {
                    unsized_types: &mut unsized_types,
                };
                visitor.visit_item_fn(func);
                if unsized_types.is_empty() {
                    continue;
                }
                let unsized_type = unsized_types[0].clone();
                let sized_bound = syn::WherePredicate::Type(syn::PredicateType {
                    lifetimes: None,
                    bounded_ty: unsized_type.clone(),
                    colon_token: syn::token::Colon::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                            paren_token: None,
                            modifier: syn::TraitBoundModifier::None,
                            lifetimes: None,
                            path: syn::Path::from(syn::Ident::new("Sized", Span::call_site())),
                        }));
                        bounds
                    },
                });
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    where_clause.predicates.push(sized_bound);
                } else {
                    func.sig.generics.where_clause = Some(syn::WhereClause {
                        where_token: syn::token::Where::default(),
                        predicates: {
                            let mut predicates = Punctuated::new();
                            predicates.push(sized_bound);
                            predicates
                        },
                    });
                }
                let mut const_expr_present = false;
                let mut const_visitor = ConstExprVisitor {
                    unsized_type: &unsized_type,
                    found: &mut const_expr_present,
                };
                const_visitor.visit_item_fn(func);
                if !const_expr_present {
                    let new_stmt: syn::Stmt = parse_quote! {
                        let _ = [(); { let _a: Option<#unsized_type> = None; 0 }];
                    };
                    func.block.stmts.insert(0, new_stmt);
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut unsized_types = Vec::new();
                        let mut visitor = UnsizedTypeVisitor {
                            unsized_types: &mut unsized_types,
                        };
                        visitor.visit_impl_item_fn(func);
                        if unsized_types.is_empty() {
                            continue;
                        }
                        let unsized_type = unsized_types[0].clone();
                        let sized_bound = syn::WherePredicate::Type(syn::PredicateType {
                            lifetimes: None,
                            bounded_ty: unsized_type.clone(),
                            colon_token: syn::token::Colon::default(),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path::from(syn::Ident::new("Sized", Span::call_site())),
                                }));
                                bounds
                            },
                        });
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            where_clause.predicates.push(sized_bound);
                        } else {
                            func.sig.generics.where_clause = Some(syn::WhereClause {
                                where_token: syn::token::Where::default(),
                                predicates: {
                                    let mut predicates = Punctuated::new();
                                    predicates.push(sized_bound);
                                    predicates
                                },
                            });
                        }
                        let mut const_expr_present = false;
                        let mut const_visitor = ConstExprVisitor {
                            unsized_type: &unsized_type,
                            found: &mut const_expr_present,
                        };
                        const_visitor.visit_impl_item_fn(func);
                        if !const_expr_present {
                            let new_stmt: syn::Stmt = parse_quote! {
                                let _ = [(); { let _a: Option<#unsized_type> = None; 0 }];
                            };
                            func.block.stmts.insert(0, new_stmt);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with where clauses. It identifies unsized types used in the function body (like str, dyn Trait, or [T]) and adds a trivially false Sized bound for that type in the where clause. If the function lacks a where clause, it creates one. Additionally, it ensures the function body contains a const expression referencing the unsized type; if not, it inserts a dummy array length expression using Option<unsized_type>. This transformation replicates conditions that trigger ICEs by combining false trivial bounds with const evaluation involving uncomputable layouts."
    }
}

struct UnsizedTypeVisitor<'a> {
    unsized_types: &'a mut Vec<syn::Type>,
}

impl<'a> syn::visit::Visit<'_> for UnsizedTypeVisitor<'a> {
    fn visit_type(&mut self, node: &syn::Type) {
        match node {
            syn::Type::Slice(_) => {
                self.unsized_types.push(node.clone());
            }
            syn::Type::TraitObject(_) => {
                self.unsized_types.push(node.clone());
            }
            syn::Type::Path(type_path) => {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident == "str" {
                        self.unsized_types.push(node.clone());
                    }
                }
            }
            _ => {}
        }
        syn::visit::visit_type(self, node);
    }
}

struct ConstExprVisitor<'a> {
    unsized_type: &'a syn::Type,
    found: &'a mut bool,
}

impl<'a> syn::visit::Visit<'_> for ConstExprVisitor<'a> {
    fn visit_expr(&mut self, node: &syn::Expr) {
        match node {
            syn::Expr::Array(array) => {
                if let Some(syn::Expr::Lit(lit)) = array.elems.last() {
                    if let syn::Lit::Int(_) = lit.lit {
                        *self.found = true;
                    }
                }
            }
            syn::Expr::Const(_) => {
                *self.found = true;
            }
            _ => {}
        }
        syn::visit::visit_expr(self, node);
    }
    fn visit_type(&mut self, node: &syn::Type) {
        if node == self.unsized_type {
            *self.found = true;
        }
        syn::visit::visit_type(self, node);
    }
}