use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80;

impl Mutator for Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if let Some(generics) = &enum_item.generics {
                    for param in &generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &param.default {
                                let out_of_range_value = match param.ty {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            segments: ref segments,
                                            ..
                                        },
                                        ..
                                    }) => {
                                        let segment = segments.first().unwrap();
                                        match segment.ident.to_string().as_str() {
                                            "u8" => 256u8,
                                            "u16" => 65536u16,
                                            "u32" => 4294967296u32,
                                            "u64" => 18446744073709551616u64,
                                            "i8" => 128i8,
                                            "i16" => 32768i16,
                                            "i32" => 2147483648i32,
                                            "i64" => 9223372036854775808i64,
                                            _ => unreachable!(),
                                        }
                                    }
                                    _ => unreachable!(),
                                };
                                let new_default = parse_quote!(#out_of_range_value);
                                param.default = Some(new_default);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes const generic parameters with out-of-range values, testing the handling of such cases by the Rust compiler. It targets enum definitions with const generics and default values calculated using expressions involving other const generic parameters."
    }
}