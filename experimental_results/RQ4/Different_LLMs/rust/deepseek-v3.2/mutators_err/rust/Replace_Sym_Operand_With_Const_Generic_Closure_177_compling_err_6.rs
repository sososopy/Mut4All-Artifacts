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

pub struct Replace_Sym_Operand_With_Const_Generic_Closure_177;

impl Mutator for Replace_Sym_Operand_With_Const_Generic_Closure_177 {
    fn name(&self) -> &str {
        "Replace_Sym_Operand_With_Const_Generic_Closure_177"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_fn_needed = false;
        let mut dummy_fn_name = Ident::new("dummy", Span::call_site());
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.generics.params.iter().any(|p| matches!(p, GenericParam::Const(_))) {
                    dummy_fn_needed = false;
                    dummy_fn_name = func.sig.ident.clone();
                }
            }
        }
        if dummy_fn_needed {
            let dummy_fn: ItemFn = parse_quote! {
                fn dummy<const N: usize>() {}
            };
            file.items.push(Item::Fn(dummy_fn));
        }
        let mut visitor = AsmVisitor {
            dummy_fn_name,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets sym operands within global_asm! and asm! macro invocations. It transforms the operand by wrapping it in a const generic argument containing an inline const block with a closure expression and literal. Specifically, replaces `sym function_name` with `sym function_name::<{ const { || {}; 0 } }>`. This complex nesting stresses the compiler's handling of inline const blocks, closures, and const generics within assembly operands, potentially triggering parsing or evaluation bugs."
    }
}

struct AsmVisitor {
    dummy_fn_name: Ident,
}

impl VisitMut for AsmVisitor {
    fn visit_expr_macro_mut(&mut self, node: &mut syn::ExprMacro) {
        if node.mac.path.segments.last().unwrap().ident == "asm" || node.mac.path.segments.last().unwrap().ident == "global_asm" {
            let mut tokens = node.mac.tokens.clone();
            let mut new_tokens = proc_macro2::TokenStream::new();
            let mut iter = tokens.into_iter();
            while let Some(token) = iter.next() {
                if let proc_macro2::TokenTree::Group(group) = token {
                    let stream = group.stream();
                    let mut inner_iter = stream.into_iter();
                    let mut new_inner = proc_macro2::TokenStream::new();
                    while let Some(inner_token) = inner_iter.next() {
                        if let proc_macro2::TokenTree::Ident(ref ident) = inner_token {
                            if ident == "sym" {
                                let next_token = inner_iter.next();
                                if let Some(proc_macro2::TokenTree::Ident(func_ident)) = next_token {
                                    let replacement = format!("sym {}::<{{ const {{ || {}; 0 }} }}", func_ident, self.dummy_fn_name);
                                    new_inner.extend(replacement.parse::<proc_macro2::TokenStream>().unwrap());
                                    continue;
                                }
                            }
                        }
                        new_inner.extend(Some(proc_macro2::TokenTree::from(inner_token)));
                    }
                    new_tokens.extend(Some(proc_macro2::TokenTree::Group(proc_macro2::Group::new(group.delimiter(), new_inner))));
                } else {
                    new_tokens.extend(Some(proc_macro2::TokenTree::from(token)));
                }
            }
            node.mac.tokens = new_tokens;
        }
        syn::visit_mut::visit_expr_macro_mut(self, node);
    }
}