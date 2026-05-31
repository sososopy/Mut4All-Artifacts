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

pub struct Replace_Awaited_Array_Size_With_Max_Usize_17;

impl Mutator for Replace_Awaited_Array_Size_With_Max_Usize_17 {
    fn name(&self) -> &str {
        "Replace_Awaited_Array_Size_With_Max_Usize_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ArrayAwaitVisitor {
            mutated: bool,
        }

        impl VisitMut for ArrayAwaitVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Await(await_expr) = expr {
                    if let Expr::Array(array_expr) = &mut *await_expr.base {
                        if let Some(len_expr) = array_expr.elems.last() {
                            if let Expr::Lit(lit_expr) = len_expr {
                                if let syn::Lit::Int(int_lit) = &lit_expr.lit {
                                    let new_token = proc_macro2::TokenStream::from_iter(vec![proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("usize", int_lit.span())), proc_macro2::TokenTree::Punct(proc_macro2::Punct::new(':', proc_macro2::Spacing::Joint)), proc_macro2::TokenTree::Punct(proc_macro2::Punct::new(':', proc_macro2::Spacing::Alone)), proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("MAX", int_lit.span()))]);
                                    lit_expr.lit = syn::Lit::Int(syn::LitInt::new("usize::MAX", int_lit.span()));
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = ArrayAwaitVisitor { mutated: false };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array expressions within async contexts that are being awaited. It modifies the size constant of an array literal to the maximum value of usize (usize::MAX). This transformation aims to trigger arithmetic overflow during size computations in the compiler's generator interior analysis, similar to the bug report. The mutation applies only when an array expression is directly awaited, preserving the element type and await suffix."
    }
}