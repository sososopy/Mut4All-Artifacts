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

pub struct Alter_Macro_With_Const_Generics_75;

impl Mutator for Alter_Macro_With_Const_Generics_75 {
    fn name(&self) -> &str {
        "Alter_Macro_With_Const_Generics_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, fields, .. }) = item {
                if let Some(GenericParam::Const(const_param)) = generics.params.first() {
                    if let Some(field) = fields.iter_mut().next() {
                        if let Type::Path(TypePath { path, .. }) = &field.ty {
                            if let Some(segment) = path.segments.last() {
                                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                    if let Some(GenericArgument::Const(Expr::Block(expr_block))) = args.args.first() {
                                        if let Some(Stmt::Expr(Expr::Macro(expr_macro), _)) = expr_block.block.stmts.first() {
                                            let new_macro = parse_quote! {
                                                macro_rules! outer_wrap {
                                                    ($x:expr) => {
                                                        #expr_macro
                                                    };
                                                }
                                            };
                                            let new_expr: Expr = parse_quote! {
                                                outer_wrap!(#const_param.ident)
                                            };
                                            *expr_macro = parse_quote! { #new_expr };
                                            file.items.insert(0, Item::Macro(new_macro));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions using const generics with macros. It introduces an additional macro layer to wrap existing macro invocations within const generic parameters. This stresses the Rust compiler's macro expansion handling in const contexts, potentially leading to internal compiler errors or unexpected behavior."
    }
}