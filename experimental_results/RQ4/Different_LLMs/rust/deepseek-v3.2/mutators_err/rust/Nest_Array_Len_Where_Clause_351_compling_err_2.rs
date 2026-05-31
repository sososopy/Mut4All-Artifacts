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

pub struct Nest_Array_Len_Where_Clause_351;

impl Mutator for Nest_Array_Len_Where_Clause_351 {
    fn name(&self) -> &str {
        "Nest_Array_Len_Where_Clause_351"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = NestArrayLenVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets where clause predicates that use array length expressions of string constant generic parameters. It wraps the original expression in a redundant nested array length expression, transforming [(); expr]: to [(); [(); expr].len()]:. This increases complexity of const evaluation in generic contexts, potentially exposing compiler bugs in nested const expression handling, type checking, or trait resolution for const generics."
    }
}

struct NestArrayLenVisitor;

impl VisitMut for NestArrayLenVisitor {
    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Fn(item_fn) => {
                self.process_generics(&mut item_fn.sig.generics);
            }
            Item::Impl(item_impl) => {
                self.process_generics(&mut item_impl.generics);
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        self.process_generics(&mut method.sig.generics);
                    }
                    if let ImplItem::Const(assoc_const) = impl_item {
                        self.process_generics(&mut assoc_const.generics);
                    }
                }
            }
            Item::Trait(item_trait) => {
                self.process_generics(&mut item_trait.generics);
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Fn(method) = trait_item {
                        self.process_generics(&mut method.sig.generics);
                    }
                    if let TraitItem::Const(assoc_const) = trait_item {
                        self.process_generics(&mut assoc_const.generics);
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, item);
    }
}

impl NestArrayLenVisitor {
    fn process_generics(&mut self, generics: &mut Generics) {
        if let Some(where_clause) = &mut generics.where_clause {
            let mut new_predicates = Punctuated::new();
            for predicate in where_clause.predicates.iter_mut() {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Type::Array(type_array) = &mut pred_type.bounded_ty {
                        if let Expr::MethodCall(method_call) = &*type_array.len {
                            if method_call.method == "len" {
                                if let Expr::Path(expr_path) = &*method_call.receiver {
                                    if expr_path.path.segments.len() == 1 {
                                        let param_name = &expr_path.path.segments[0].ident;
                                        if generics.params.iter().any(|param| {
                                            if let GenericParam::Const(const_param) = param {
                                                const_param.ident == *param_name
                                                    && if let Type::Reference(type_ref) = &const_param.ty {
                                                        if let Type::Path(type_path) = &*type_ref.elem {
                                                            type_path.path.segments.len() == (if type_ref.mutability.is_some() { 2 } else { 1 })
                                                                && type_path.path.segments.last().unwrap().ident == "str"
                                                        } else {
                                                            false
                                                        }
                                                    } else {
                                                        false
                                                    }
                                            } else {
                                                false
                                            }
                                        }) {
                                            let new_expr: Expr = parse_quote! { [(); #method_call].len() };
                                            type_array.len = Box::new(new_expr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                new_predicates.push(predicate.clone());
            }
            where_clause.predicates = new_predicates;
        }
    }
}