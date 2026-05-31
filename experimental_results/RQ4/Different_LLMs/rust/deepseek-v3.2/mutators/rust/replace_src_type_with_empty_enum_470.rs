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

pub struct Replace_Src_Type_With_Empty_Enum_470;

impl Mutator for Replace_Src_Type_With_Empty_Enum_470 {
    fn name(&self) -> &str {
        "Replace_Src_Type_With_Empty_Enum_470"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut empty_enum_name = None;
        let mut visitor = FindEmptyEnumVisitor {
            empty_enum_name: &mut empty_enum_name,
        };
        visitor.visit_file_mut(file);
        
        let empty_enum_ident = if let Some(name) = empty_enum_name {
            name.clone()
        } else {
            let new_name = Ident::new("MutEmptyEnum", Span::call_site());
            let new_enum: Item = parse_quote! {
                pub enum MutEmptyEnum {}
            };
            file.items.insert(0, new_enum);
            new_name
        };
        
        let mut visitor = ReplaceSrcTypeVisitor {
            empty_enum_ident: empty_enum_ident.clone(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets transmutability assertions using `std::mem::TransmuteFrom`. It replaces the source type argument (`Src`) with a reference to an empty enum type to trigger layout computation for zero-variant enums. If the seed contains an empty enum, it uses that; otherwise, it adds a new empty enum at the crate root. This mutation aims to expose internal compiler errors during transmutability analysis when handling empty enums, specifically targeting subtraction overflow bugs in layout calculation."
    }
}

struct FindEmptyEnumVisitor<'a> {
    empty_enum_name: &'a mut Option<Ident>,
}

impl<'a> VisitMut for FindEmptyEnumVisitor<'a> {
    fn visit_item_mut(&mut self, item: &mut Item) {
        if let Item::Enum(item_enum) = item {
            if item_enum.variants.is_empty() {
                *self.empty_enum_name = Some(item_enum.ident.clone());
            }
        }
        visit_mut::visit_item_mut(self, item);
    }
}

struct ReplaceSrcTypeVisitor {
    empty_enum_ident: Ident,
}

impl VisitMut for ReplaceSrcTypeVisitor {
    fn visit_expr_call_mut(&mut self, expr: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &mut *expr.func {
            if let Some(last_segment) = path.segments.last_mut() {
                if last_segment.ident == "is_transmutable" {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        if let Some(first_arg) = args.args.first_mut() {
                            if let GenericArgument::Type(_) = first_arg {
                                let empty_enum_ident = self.empty_enum_ident.clone();
                                let new_type: Type = parse_quote! { #empty_enum_ident };
                                *first_arg = GenericArgument::Type(new_type);
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, expr);
    }
}