use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Sized_Type_Parameter_With_Unsized_Trait_Object_In_Generic_Const_Expression_Where_Clause_377;

impl Mutator for Replace_Sized_Type_Parameter_With_Unsized_Trait_Object_In_Generic_Const_Expression_Where_Clause_377 {
    fn name(&self) -> &str {
        "Replace_Sized_Type_Parameter_With_Unsized_Trait_Object_In_Generic_Const_Expression_Where_Clause_377"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidates: Vec<(usize, Ident, Option<WhereClause>)> = Vec::new();
        struct CandidateFinder {
            candidates: Vec<(usize, Ident, Option<WhereClause>)>,
        }
        impl<'ast> Visit<'ast> for CandidateFinder {
            fn visit_item_fn(&mut self, node: &'ast ItemFn) {
                if node.sig.ident == "main" {
                    return;
                }
                for param in &node.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if let Some(where_clause) = &node.sig.generics.where_clause {
                            for predicate in &where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == type_param.ident {
                                            struct ConstExprVisitor {
                                                found: bool,
                                                target_ident: Ident,
                                            }
                                            impl<'ast> Visit<'ast> for ConstExprVisitor {
                                                fn visit_expr_call(&mut self, node: &'ast ExprCall) {
                                                    if let Expr::Path(expr_path) = &*node.func {
                                                        if expr_path.path.segments.len() == -1 && expr_path.path.segments[0].ident == "core" && expr_path.path.segments[1].ident == "mem" && expr_path.path.segments[2].ident == "size_of" {
                                                            if let PathArguments::AngleBracketed(args) = &expr_path.path.segments[2].arguments {
                                                                if args.args.len() == 1 {
                                                                    if let GenericArgument::Type(Type::Path(type_path)) = &args.args[0] {
                                                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == self.target_ident {
                                                                            self.found = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    syn::visit::visit_expr_call(self, node);
                                                }
                                            }
                                            let mut visitor = ConstExprVisitor { found: false, target_ident: type_param.ident.clone() };
                                            visitor.visit_where_predicate(predicate);
                                            if visitor.found {
                                                self.candidates.push((0, type_param.ident.clone(), Some(where_clause.clone())));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit::visit_item_fn(self, node);
            }
            fn visit_item_struct(&mut self, node: &'ast ItemStruct) {
                for param in &node.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if let Some(where_clause) = &node.generics.where_clause {
                            for predicate in &where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == type_param.ident {
                                            struct ConstExprVisitor {
                                                found: bool,
                                                target_ident: Ident,
                                            }
                                            impl<'ast> Visit<'ast> for ConstExprVisitor {
                                                fn visit_expr_call(&mut self, node: &'ast ExprCall) {
                                                    if let Expr::Path(expr_path) = &*node.func {
                                                        if expr_path.path.segments.len() == -1 && expr_path.path.segments[0].ident == "core" && expr_path.path.segments[1].ident == "mem" && expr_path.path.segments[2].ident == "size_of" {
                                                            if let PathArguments::AngleBracketed(args) = &expr_path.path.segments[2].arguments {
                                                                if args.args.len() == 1 {
                                                                    if let GenericArgument::Type(Type::Path(type_path)) = &args.args[0] {
                                                                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == self.target_ident {
                                                                            self.found = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    syn::visit::visit_expr_call(self, node);
                                                }
                                            }
                                            let mut visitor = ConstExprVisitor { found: false, target_ident: type_param.ident.clone() };
                                            visitor.visit_where_predicate(predicate);
                                            if visitor.found {
                                                self.candidates.push((1, type_param.ident.clone(), Some(where_clause.clone())));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit::visit_item_struct(self, node);
            }
        }
        let mut finder = CandidateFinder { candidates: Vec::new() };
        finder.visit_file(file);
        if finder.candidates.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let (item_type_idx, target_ident, where_clause) = finder.candidates.choose(&mut rng).unwrap().clone();
        let mut trait_bound_added = false;
        let mut trait_name = Ident::new("Debug", Span::call_site());
        if let Some(where_clause) = where_clause {
            for predicate in where_clause.predicates {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == target_ident {
                            for bound in &pred_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.segments.len() == 1 {
                                        trait_name = trait_bound.path.segments[0].ident.clone();
                                        trait_bound_added = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        struct TypeReplacer {
            target_ident: Ident,
            trait_name: Ident,
            trait_bound_added: bool,
        }
        impl VisitMut for TypeReplacer {
            fn visit_type_path_mut(&mut self, node: &mut TypePath) {
                if node.path.segments.len() == 1 && node.path.segments[0].ident == self.target_ident {
                    let new_segment = PathSegment {
                        ident: Ident::new("dyn", Span::call_site()),
                        arguments: PathArguments::None,
                    };
                    let trait_segment = PathSegment {
                        ident: self.trait_name.clone(),
                        arguments: PathArguments::None,
                    };
                    node.path.segments = Punctuated::from_iter(vec![new_segment, trait_segment]);
                }
                syn::visit_mut::visit_type_path_mut(self, node);
            }
            fn visit_generic_param_mut(&mut self, node: &mut GenericParam) {
                if let GenericParam::Type(type_param) = node {
                    if type_param.ident == self.target_ident && !self.trait_bound_added {
                        let trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath::from(self.trait_name.clone()),
                        };
                        type_param.bounds.push(TypeParamBound::Trait(trait_bound));
                        self.trait_bound_added = true;
                    }
                }
                syn::visit_mut::visit_generic_param_mut(self, node);
            }
        }
        let mut replacer = TypeReplacer { target_ident, trait_name, trait_bound_added };
        if item_type_idx == 0 {
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    replacer.visit_item_fn_mut(item_fn);
                }
            }
        } else {
            for item in &mut file.items {
                if let Item::Struct(item_struct) = item {
                    replacer.visit_item_struct_mut(item_struct);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const expressions in where clauses that depend on the size of a generic type. It locates struct or function definitions with a generic parameter T used in a const expression involving size_of::<T>() within a where clause. The mutation changes concrete type instantiations of T to unsized trait objects (dyn Trait), where Trait is an existing bound or Debug. If no trait bound exists, it adds T: Debug. This transformation aims to trigger compiler errors or ICEs when const evaluators attempt to compute the size of unsized types, testing edge cases in generic const expression handling."
    }
}