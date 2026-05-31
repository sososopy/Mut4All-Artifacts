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

pub struct Replace_Const_Expr_With_Invalid_Closure_370;

impl Mutator for Replace_Const_Expr_With_Invalid_Closure_370 {
    fn name(&self) -> &str {
        "Replace_Const_Expr_With_Invalid_Closure_370"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstGenericVisitor {
            should_mutate: bool,
        }

        impl VisitMut for ConstGenericVisitor {
            fn visit_generic_param_mut(&mut self, i: &mut GenericParam) {
                if let GenericParam::Const(c) = i {
                    if let Some(expr) = &mut c.default {
                        let closure_expr = parse_quote! { || {} };
                        *expr = closure_expr;
                    }
                }
            }

            fn visit_item_mut(&mut self, node: &mut Item) {
                self.should_mutate = true;
                syn::visit_mut::visit_item_mut(self, node);
            }

            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                if self.should_mutate {
                    syn::visit_mut::visit_item_fn_mut(self, i);
                }
            }

            fn visit_item_struct_mut(&mut self, i: &mut ItemStruct) {
                if self.should_mutate {
                    syn::visit_mut::visit_item_struct_mut(self, i);
                }
            }

            fn visit_item_enum_mut(&mut self, i: &mut ItemEnum) {
                if self.should_mutate {
                    syn::visit_mut::visit_item_enum_mut(self, i);
                }
            }
        }

        let mut visitor = ConstGenericVisitor {
            should_mutate: true,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}