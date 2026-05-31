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

pub struct Replace_Dyn_Star_With_Box_33;

impl Mutator for Replace_Dyn_Star_With_Box_33 {
    fn name(&self) -> &str {
        "Replace_Dyn_Star_With_Box_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::TraitObject(type_trait_object) = &**return_type {
                        if type_trait_object.dyn_token.is_some() {
                            let new_type: Type = parse_quote! { Box<dyn Debug> };
                            *return_type = Box::new(new_type);
                        }
                    }
                }
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Cast(expr_cast)) = stmt {
                        if let Type::TraitObject(type_trait_object) = &*expr_cast.ty {
                            if type_trait_object.dyn_token.is_some() {
                                expr_cast.ty = Box::new(parse_quote! { Box<dyn Debug> });
                                if let Expr::Call(expr_call) = &mut *expr_cast.expr {
                                    expr_call.func = Box::new(parse_quote! { Box::new });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies `dyn*` type casts in function return types and expressions, replacing them with `Box<dyn Debug>`. This transformation tests the compiler's handling of explicit type conversions and ensures compatibility with traditional type casting mechanisms, potentially exposing issues in type inference and trait object handling."
    }
}