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

pub struct Replace_Const_Generic_Parameters_With_Different_Types_350;

impl Mutator for Replace_Const_Generic_Parameters_With_Different_Types_350 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameters_With_Different_Types_350"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let new_type = match rng.gen_range(0..3) {
                                0 => parse_quote!(u8),
                                1 => parse_quote!(i32),
                                2 => parse_quote!(usize),
                                _ => unreachable!(),
                            };
                            param.ty = new_type;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with different types (u8, i32, usize) in struct definitions. This transformation tests the compiler's handling of different types in const generic parameters and can help reveal bugs related to type parameter handling and substitution."
    }
}