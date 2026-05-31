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

pub struct Enclose_Enum_In_Function_With_Unused_Type_Parameter_644;

impl Mutator for Enclose_Enum_In_Function_With_Unused_Type_Parameter_644 {
    fn name(&self) -> &str {
        "Enclose_Enum_In_Function_With_Unused_Type_Parameter_644"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_enum = false;
                for stmt in &func.block.stmts {
                    if let Stmt::Item(Item::Enum(_)) = stmt {
                        has_enum = true;
                        break;
                    }
                }
                if has_enum {
                    let new_generic: GenericParam = parse_quote!(U);
                    func.sig.generics.params.push(new_generic);

                    for stmt in &mut func.block.stmts {
                        if let Stmt::Item(Item::Enum(enum_item)) = stmt {
                            if let Some(variant) = enum_item.variants.first_mut() {
                                variant.fields = syn::Fields::Unnamed(syn::FieldsUnnamed {
                                    paren_token: token::Paren::default(),
                                    unnamed: {
                                        let mut fields = Punctuated::new();
                                        fields.push(parse_quote!(U));
                                        fields
                                    },
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}