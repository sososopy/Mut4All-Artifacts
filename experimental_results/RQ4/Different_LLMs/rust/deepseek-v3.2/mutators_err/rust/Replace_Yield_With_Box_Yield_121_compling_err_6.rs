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

pub struct Replace_Yield_With_Box_Yield_121;

impl Mutator for Replace_Yield_With_Box_Yield_121 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Box_Yield_121"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_box_syntax = false;
        for attr in &file.attrs {
            if let Meta::List(list) = &attr.meta {
                for token in list.tokens.clone() {
                    if token.to_string().contains("box_syntax") {
                        has_box_syntax = true;
                        break;
                    }
                }
            }
        }
        if !has_box_syntax {
            file.attrs.push(parse_quote!(#![feature(box_syntax)]));
        }

        struct YieldReplacer {
            mutated: bool,
        }

        impl VisitMut for YieldReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Yield(yield_expr) = expr {
                    if !self.mutated {
                        let box_expr = parse_quote!(box #yield_expr);
                        *expr = box_expr;
                        self.mutated = true;
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut replacer = YieldReplacer { mutated: false };
                replacer.visit_block_mut(&mut item_fn.block);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(item_fn) = impl_item {
                        let mut replacer = YieldReplacer { mutated: false };
                        replacer.visit_block_mut(&mut item_fn.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generator closures containing yield expressions. It replaces the first yield expression encountered with a box expression containing the yield, transforming `yield` into `box yield`. This mutation requires the box_syntax feature; if not present, it adds #![feature(box_syntax)] at the crate root. The transformation aims to trigger MIR validation errors related to type mismatches in generator state handling by introducing boxed yield values."
    }
}