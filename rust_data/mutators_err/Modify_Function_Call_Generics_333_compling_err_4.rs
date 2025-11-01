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

pub struct Modify_Function_Call_Generics_333;

impl Mutator for Modify_Function_Call_Generics_333 {
    fn name(&self) -> &str {
        "Modify_Function_Call_Generics_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let fn_generics = &item_fn.sig.generics.params;
                let fn_generic_count = fn_generics.len();

                let mut visitor = FunctionCallVisitor {
                    fn_generic_count,
                    fn_generics: fn_generics.clone(),
                };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function calls that use generic parameters. It checks if the function call provides fewer generic type arguments than the function definition requires. If so, it adds the missing generic type argument(s) to the function call to match the function definition's requirements. This ensures that the function calls are consistent with their definitions and can help uncover hidden bugs or assumptions in the code."
    }
}

struct FunctionCallVisitor {
    fn_generic_count: usize,
    fn_generics: Punctuated<GenericParam, Comma>,
}

impl VisitMut for FunctionCallVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            let mut path = path.clone();
            if let Some(segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                    let current_count = args.args.len();
                    if current_count < self.fn_generic_count {
                        for _ in current_count..self.fn_generic_count {
                            if let Some(GenericParam::Type(type_param)) = self.fn_generics.iter().nth(current_count) {
                                let new_generic = parse_quote! { #type_param };
                                args.args.push(GenericArgument::Type(new_generic));
                            }
                        }
                    }
                }
            }
            node.func = Box::new(Expr::Path(ExprPath { path, attrs: Vec::new(), qself: None }));
        }
        visit_mut::visit_expr_call_mut(self, node);
    }
}