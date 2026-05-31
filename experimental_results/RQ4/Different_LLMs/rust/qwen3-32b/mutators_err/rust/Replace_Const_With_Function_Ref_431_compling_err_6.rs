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

pub struct Replace_Const_With_Function_Ref_431;

impl Mutator for Replace_Const_With_Function_Ref_431 {
    fn name(&self) -> &str {
        "Replace_Const_With_Function_Ref_431"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_names = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                function_names.push(item_fn.sig.ident.to_string());
            }
        }
        if function_names.is_empty() {
            return;
        }
        let mut visitor = ReplaceConstWithFunctionVisitor {
            function_names,
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ReplaceConstWithFunctionVisitor {
    function_names: Vec<String>,
    rng: rand::rngs::ThreadRng,
}

impl ReplaceConstWithFunctionVisitor {
    fn select_random_function(&mut self) -> String {
        let name = self.function_names.choose(&mut self.rng).unwrap().clone();
        name
    }
}

impl<'ast> VisitMut for ReplaceConstWithFunctionVisitor {
    fn visit_type_array_mut(&mut self, i: &mut TypeArray) {
        let selected_name = self.select_random_function();
        let path = parse_quote!(#selected_name);
        i.len = Expr::Path(ExprPath {
            attrs: vec![],
            qself: None,
            path,
        });
        syn::visit_mut::visit_type_array_mut(self, i);
    }

    fn visit_item_mut(&mut self, i: &mut Item) {
        if let Item::Struct(i) = i {
            let generics = &mut i.generics;
            for param in &mut generics.params {
                if let GenericParam::Const(const_param) = param {
                    if const_param.eq_token.is_none() {
                        let selected_name = self.select_random_function();
                        let path = parse_quote!(#selected_name);
                        let expr = Box::new(Expr::Path(ExprPath {
                            attrs: vec![],
                            qself: None,
                            path,
                        }));
                        const_param.default = Some(expr);
                    }
                }
            }
        }
        syn::visit_mut::visit_item_mut(self, i);
    }
}