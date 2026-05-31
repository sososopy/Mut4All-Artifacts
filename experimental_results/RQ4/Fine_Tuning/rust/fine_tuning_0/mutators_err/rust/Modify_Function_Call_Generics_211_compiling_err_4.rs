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

pub struct Modify_Function_Call_Generics_211;

impl Mutator for Modify_Function_Call_Generics_211 {
    fn name(&self) -> &str {
        "Modify_Function_Call_Generics_211"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut function_definitions = HashSet::new();

        for item in &file.items {
            if let syn::Item::Fn(func) = item {
                let func_name = func.sig.ident.to_string();
                let required_generics = func.sig.generics.params.len();
                function_definitions.insert((func_name, required_generics));
            }
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = FunctionCallVisitor {
                    function_definitions: &function_definitions,
                };
                visitor.visit_item_fn_mut(func);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function calls with insufficient generic arguments and modifies them to provide the missing arguments. This transformation ensures that function calls match their definitions in terms of generic parameters, potentially revealing issues related to generic argument handling."
    }
}

struct FunctionCallVisitor<'a> {
    function_definitions: &'a HashSet<(String, usize)>,
}

impl<'a> VisitMut for FunctionCallVisitor<'a> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if let Some(last_segment) = path.segments.last_mut() {
                let func_name = last_segment.ident.to_string();
                if let Some(&(ref name, required_generics)) = self.function_definitions.iter().find(|&&(ref n, _)| n == &func_name) {
                    if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                        let current_generics = args.args.len();
                        if current_generics < required_generics {
                            let missing_generics = required_generics - current_generics;
                            for _ in 0..missing_generics {
                                args.args.push(GenericArgument::Type(parse_quote!(i32)));
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}