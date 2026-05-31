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

pub struct Introduce_Const_Generic_Where_Clause_88;

impl Mutator for Introduce_Const_Generic_Where_Clause_88 {
    fn name(&self) -> &str {
        "Introduce_Const_Generic_Where_Clause_88"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = IntroduceConstGenericWhereClauseVisitor {
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a new struct definition with a const generic parameter and a where clause that uses the const parameter in a bound, such as [(); const_param.len()]: . It targets functions or associated constants that already use const generic parameters of type &'static str (or other types supporting .len()). The new struct is placed in the innermost scope possible (e.g., inside a block or another struct) and is never instantiated. This mutation stresses the compiler's const evaluation during type checking, particularly for nested definitions and where clauses involving const generics, potentially exposing MIR generation bugs."
    }
}

struct IntroduceConstGenericWhereClauseVisitor {
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for IntroduceConstGenericWhereClauseVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        let mut const_params = Vec::new();
        for param in &i.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                const_params.push(const_param);
            }
        }
        if const_params.is_empty() {
            return;
        }
        let chosen_param = const_params.choose(&mut self.rng);
        if let Some(param) = chosen_param {
            let param_ident = &param.ident;
            let param_type = &param.ty;
            let where_clause_predicate: syn::WherePredicate = parse_quote!([(); #param_ident.len()]:);
            let new_struct: ItemStruct = parse_quote! {
                struct Nested<const #param_ident: #param_type> where #where_clause_predicate {}
            };
            let new_stmt = Stmt::Item(Item::Struct(new_struct));
            i.block.stmts.insert(0, new_stmt);
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }

    fn visit_item_const_mut(&mut self, i: &mut ItemConst) {
        if let Type::Path(type_path) = &*i.ty {
            if let Some(segment) = type_path.path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    for arg in &args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Path(expr_path) = const_arg {
                                let param_ident = &expr_path.path.segments.last().unwrap().ident;
                                let param_type = &segment.ident;
                                let where_clause_predicate: syn::WherePredicate = parse_quote!([(); #param_ident.len()]:);
                                let new_struct: ItemStruct = parse_quote! {
                                    struct Nested<const #param_ident: #param_type> where #where_clause_predicate {}
                                };
                                let new_stmt = Stmt::Item(Item::Struct(new_struct));
                                let mut block: syn::Block = parse_quote!({});
                                block.stmts.push(new_stmt);
                                i.expr = Box::new(Expr::Block(syn::ExprBlock { attrs: Vec::new(), block, label: None }));
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_item_const_mut(self, i);
    }

    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        for impl_item in &mut i.items {
            if let ImplItem::Fn(func) = impl_item {
                let mut item_fn: ItemFn = func.clone();
                self.visit_item_fn_mut(&mut item_fn);
                *func = ImplItem::Fn(item_fn);
            }
            if let ImplItem::Const(const_item) = impl_item {
                let mut item_const: ItemConst = const_item.clone();
                self.visit_item_const_mut(&mut item_const);
                *const_item = ImplItem::Const(item_const);
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, i);
    }
}