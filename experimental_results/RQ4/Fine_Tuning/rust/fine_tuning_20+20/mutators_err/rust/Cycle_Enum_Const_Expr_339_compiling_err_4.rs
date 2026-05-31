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

pub struct Cycle_Enum_Const_Expr_339;

impl Mutator for Cycle_Enum_Const_Expr_339 {
    fn name(&self) -> &str {
        "Cycle_Enum_Const_Expr_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_const_expr_variant = false;
                for variant in &item_enum.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        if let Expr::Block(expr_block) = expr {
                            if let Some(block_expr) = expr_block.block.stmts.last() {
                                if let Stmt::Expr(Expr::Path(expr_path), _) = block_expr {
                                    if expr_path
                                        .path
                                        .segments
                                        .iter()
                                        .any(|seg| seg.ident == item_enum.ident)
                                    {
                                        has_const_expr_variant = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if has_const_expr_variant {
                    for variant in &mut item_enum.variants {
                        if let Some((_, expr)) = &mut variant.discriminant {
                            if let Expr::Block(expr_block) = expr {
                                if let Some(block_expr) = expr_block.block.stmts.last() {
                                    if let Stmt::Expr(Expr::Path(expr_path), _) = block_expr {
                                        if expr_path
                                            .path
                                            .segments
                                            .iter()
                                            .any(|seg| seg.ident == item_enum.ident)
                                        {
                                            let variant_ident = variant.ident.clone();
                                            let new_expr: Expr = parse_quote! {
                                                {
                                                    #item_enum::#variant_ident as u8 - #item_enum::#variant_ident as u8
                                                }
                                            };
                                            *expr = new_expr;
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
        "This mutator targets enums with constant expressions that reference their own variants. It modifies such expressions to introduce a cyclic reference, aiming to create compile-time evaluation issues by leveraging self-referential arithmetic operations."
    }
}